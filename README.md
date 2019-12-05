# BlackJackGame
Group project developing a blackjack server-client game using Boost::asio and GTK gui.

# HOW TO RUN
1. run `./server 9000` in one terminal

2. run `./client 127.0.0.1 9000` other terminal(s)

Enter name and bet amount for client dialog- gui would pop up.
There 10 seconds after first client joins, for other clients to join
If the any client that joined the game has not yet finished the dialog, more players can still join.
The limit of one game is six players. No more than 6 players are allowed.
When the game starts, there will be a message in status bar showing whose turn it is.

New Game:
    When every player has taken his turn, dealer will draw its cards and results will be displayed in a dialog, after closing dialog, results will be displayed in statusbar.
    New game can be started by going to File->New Game or clicking the new game icon.
    If any new player joins when game is ended, new game will be started for all.
    If any player clicks New Game, new game will be started for all.
    If new game starts, each player will be shown a dialog to enter their bet. Closing off the dialog or pressing leave button will be treated as player's incentive to leave and will be disconnected.
