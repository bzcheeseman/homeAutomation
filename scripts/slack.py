#
# Created by Aman LaChapelle on 8/14/16.
#


import slacker as sl
import logging

## @package slack
#  Holds the SlackServer object, subpackage to scripts for convenience.

## @var level
# The logging level
## @var format
# The logfile format
## @var filename
# The name for the logfile
## @var filemode
# The file mode for the logfile

logging.basicConfig(level=logging.INFO,
                    format = '%(asctime)s %(threadName)s %(name)s %(levelname)s %(message)s',
                    filename = "/Users/Aman/code/homeAutomation/logging/slack_py.log",
                    filemode = 'w')

logging.getLogger('requests').setLevel(logging.WARNING)

## @brief The base object that controls the interface to Slack.
#
# Holds methods for reading messages, sending messages and closing the interface remotely.  Generally tried to make this
# as minimal as possible.
class SlackServer(object):

    ## @var log
    # Holds the logging object.
    ## @var requests
    # Holds the number of requests so we can check in on the server.
    ## @var slack
    # Holds the slack object for talking to Slack.
    ## @var me
    # Holds the bot's ID.
    ## @var general
    # Holds the general channel's ID.
    ## @var last_read
    # Holds the timestamp of the last message read.  Used to determine term start/end in C
    ## @var commands
    # The commands logfile.
    ## @var quiet
    # Keeps the server quiet if that's what's needed - no online message.

    ## Constructs the SlackServer object.  Connects to Slack and posts an online message.
    #
    # @param token The slack token used to connect to Slack
    # @param quiet Makes the bot operate quietly in the background.
    def __init__(self, token, quiet=False):

        self.requests = 0

        self.quiet = quiet

        # token = "xoxb-69173535956-1Gyvty58l8QHqhUVxvSVCt6s"


        self.log = logging.getLogger("SlackServer")

        self.slack = sl.Slacker(token)
        self.me = self.slack.users.get_user_id("homebot")
        self.general = self.slack.channels.get_channel_id("general")
        self.last_read = None

        self.commands = open("/Users/Aman/code/homeAutomation/logging/commands.log", 'w')


        ok = self.slack.api.test()
        if ok.body['ok'] and not self.quiet:
            onlineMsg = self.slack.chat.post_message(channel="#general", text="Home pybot Online!", as_user=self.me)
            self.last_read = onlineMsg.body['ts']
            self.log.info("Online at %s" % self.last_read)
        elif ok.body['ok'] and self.quiet:
            lr = self.slack.channels.info(self.general)
            self.last_read = lr.body['channel']['last_read']
            self.log.info("Online at %s" % self.last_read)
        else:
            print "Uh Oh!  Not connected!"

        self.last_read = float(self.last_read)

    ## Sends a message
    #
    # @param msg The message to send out
    def send(self, msg):
        self.slack.chat.post_message("#general", msg, as_user=self.me)

    ## The main handler for messages: this reads all messages not yet read and takes ones addressed to the bot
    #  and deals with them.
    #
    # @return last_read, used to determine the term start/end in C.  Returns -1.0 on user exit.
    def handle(self):
        msgs = self.slack.channels.history(channel=self.general, oldest=self.last_read)

        for message in msgs.body["messages"]:
            msg = message['text'].split(" ")
            if msg[0] == "<@%s>" % self.me:
                msg = " ".join(msg[1:])
                self.log.info("Received message: %s" % msg)
                self.last_read = message['ts']
                if msg[0:2] == "//":
                    cmd = msg[2:].lower()
                    if cmd == "close":
                        self.commands.close()
                        self.send("Shutting down now, goodbye!")
                        self.log.info("Shutting down after %s" % self.last_read)
                        return float(-1.0)
                        # return float(self.last_read)
                    self.commands.write(cmd + "\n")
                    self.commands.flush()
                    self.log.info("Received command %s" % cmd)
                    self.requests += 1


                elif msg == "how are you?":
                    self.send("I'm good!  I've handled {} requests so far this session.".format(self.requests))

        return float(self.last_read)