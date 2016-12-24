//
// Created by Aman LaChapelle on 12/20/16.
//
// homeAutomation
// Copyright (c) 2016 Aman LaChapelle
// Full license at homeAutomation/LICENSE.txt
//

/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "../include/resnet.hpp"

resnet::resnet(const std::string &sync_folder) : sync_folder(sync_folder),
                                                 fnt("../../data/English_train", dataset::FONT),
                                                 hnd("../../data/English_train", dataset::HAND) {
  while (!fnt.ready() && !hnd.ready()){
    dlib::sleep(10);
  }
}

resnet::~resnet() {
  ;
}

void resnet::train(dataset::data_folder_t which, bool force_retrain) {

  size_t train_size, test_size;
  dataset *this_run;

  switch(which){
    case dataset::FONT : this_run = &fnt;
      train_size = fnt.size_train();
      test_size = fnt.size_test();
      break;
    case dataset::HAND : this_run = &hnd;
      train_size = hnd.size_train();
      test_size = hnd.size_test();
      break;
  }

  try{
    if (force_retrain){
      std::remove("../../logging/resnet_logs/train.dat");
      std::remove("../../logging/resnet_logs/predict.dat");
      std::remove("../../logging/resnet_logs/train_sync.dat");
      throw std::runtime_error ("forced retrain");
    }

    deserialize("../../logging/resnet_logs/predict.dat") >> predicting;

    std::cout << "Reading pretrained model" << std::endl;
  }
  catch(...){ //no serialized network
    try {
      set_dnn_prefer_smallest_algorithms();

      dnn_trainer<resnet_train, adam> trainer(training, adam(0.0005, 0.9, 0.999));

      trainer.be_verbose();

      trainer.set_synchronization_file(sync_folder + "/train_sync.dat", std::chrono::seconds(100));

      trainer.set_iterations_without_progress_threshold(3000);
      trainer.set_learning_rate_shrink_factor(0.1);
      trainer.set_learning_rate(1e-3);

      std::vector<matrix<unsigned char>> mini_batch_samples;
      std::vector<unsigned long> mini_batch_labels;

      matrix<unsigned char> temp_img;
      unsigned long temp_label;

      dlib::rand rnd(time(0));
      while (trainer.get_learning_rate() >= 1e-6) {
        mini_batch_samples.clear();
        mini_batch_labels.clear();

        // make a 64 image mini-batch
        while (mini_batch_samples.size() < 64) {
          auto idx = rnd.get_random_32bit_number() % train_size;
          (*this_run)(idx, temp_img, temp_label, false);
          mini_batch_samples.push_back(temp_img);
          mini_batch_labels.push_back(temp_label);
        }

        trainer.train_one_step(mini_batch_samples, mini_batch_labels);
      }

      trainer.get_net();

      std::cout << "Training Finished" << std::endl;

      training.clean();
      serialize(sync_folder + "/train.dat") << training;

      predicting = training;
      serialize(sync_folder + "/predict.dat") << predicting;
    }
    catch(std::exception &e){
      std::cout << e.what() << std::endl;
    }
  } //no serialized network

  try{

    std::vector<matrix<unsigned char>> current_images;
    std::vector<unsigned long> current_labels;

    (*this_run)(0, train_size, current_images, current_labels, false);

    std::vector<unsigned long> predicted_labels = predicting(current_images);

    std::vector<int> right (number_of_classes, 0);
    std::vector<int> wrong (number_of_classes, 0);
    std::vector<float> linspace (number_of_classes);

    for (int i = 0; i < number_of_classes; i++){
      linspace[i] = (float)i+1;
    }

    for (size_t i = 0; i < train_size; ++i) {
      if (predicted_labels[i] == current_labels[i])
        right[current_labels[i]]++;
      else
        wrong[current_labels[i]]++;
    }

    int num_right = std::accumulate(right.begin(), right.end(), 0);
    int num_wrong = std::accumulate(wrong.begin(), wrong.end(), 0);

    std::cout << "Training right: " << right << std::endl;
    std::cout << "Training wrong: " << wrong << std::endl;
    std::cout << "Training total right: " << num_right << std::endl;
    std::cout << "Training total wrong: " << num_wrong << std::endl;
    std::cout << "Training accuracy:  " << num_right/(double)(num_right+num_wrong) << std::endl;

    pycpp::which_python = "/usr/local/bin/ipython";

    pycpp::py_module p ("matplotlib");
    p("use", 1, pycpp::to_python("TkAgg"));

    pycpp::py_module py ("matplotlib.pyplot");

    py("bar",
       pycpp::make_tuple(2, pycpp::to_python(linspace), pycpp::to_python(wrong)),
       pycpp::make_dict(2, pycpp::to_python("label"), pycpp::to_python("Training"))
    );

    current_images.clear();
    current_labels.clear();

    (*this_run)(0, test_size, current_images, current_labels, true);

    predicted_labels = predicting(current_images);
    right.assign(number_of_classes, 0);
    wrong.assign(number_of_classes, 0);

    for (size_t i = 0; i < test_size; ++i)
    {
      if (predicted_labels[i] == current_labels[i])
        right[current_labels[i]]++;
      else
        wrong[current_labels[i]]++;

    }

    num_right = std::accumulate(right.begin(), right.end(), 0);
    num_wrong = std::accumulate(wrong.begin(), wrong.end(), 0);

    std::cout << "Testing right: " << right << std::endl;
    std::cout << "Testing wrong: " << wrong << std::endl;
    std::cout << "Testing total right: " << num_right << std::endl;
    std::cout << "Testing total wrong: " << num_wrong << std::endl;
    std::cout << "Testing accuracy:  " << num_right/(double)(num_right+num_wrong) << std::endl;

    std::for_each(linspace.begin(), linspace.end(), [](float f){f += 0.35;});

    py("bar",
       pycpp::make_tuple(2, pycpp::to_python(linspace), pycpp::to_python(wrong)),
       pycpp::make_dict(2, pycpp::to_python("label"), pycpp::to_python("Testing"))
    );

    py("legend", 0);
    py("show", 0);
  }
  catch(std::exception &e){
    std::cout << e.what() << std::endl;
  }

}
