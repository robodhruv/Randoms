Trying out this amazing framework!


To run, follow the installation instructions given [here](https://github.com/openai/universe/blob/master/README.rst#installation).

To core of the game is in the file `main.py`. To run, launch a terminal and launch *Docker* as follows:

```
sudo docker run -p 5900:5900 -p 15901:15901 --privileged --cap-add SYS_ADMIN --ipc host quay.io/openai/universe.flashgames:0.20.28

```

Then launch another terminal to run the game.  
```
sudo python main.py
```

__Now sit back and ENJOY!__
