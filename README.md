# Random Bab Generator

<p align="center">
  <img src="./img/randombab.png" alt="Alt Text" width="150">
</p>

**Project Motivation** :

매 주 목요일마다 있는 랜덤밥 그룹을 조금 더 형평성에 맞게!

매니저님들 손 안 아프게! 하기 위한 자동화 프로그램

## _How to Run_

##### Prerequisites:

You need jansson, curl, cmake, virtualenv installed.

⚠️ You need your own app & bot token from Slack. Change the app token and the bot token in the .env file. ⚠️
⚠️ Your Slack bot must be in SOCKET MODE. ⚠️

#### _MacOS_

```
$ git clone https://github.com/yeseongmoon/random_bab_generator.git
$ cd random_bab_generator/
$ cmake build/
$ cmake --build build/
$ cd src/
$ python3 -m venv .your_venv_name
$ source .your_venv_name/bin/activate
$ pip install -r requirements.txt
$ python3 slack_bot.py
```

The Slack bot for your Slack workspace is now waiting for you to enter your custom command! (e.g., /randbab)

#### _Windows_

```
Ask Chan-Woo Ban Jang!
```

#### _Linux_

```
Not yet tried! :-)
```

## _How to use?_

The basic command is /randbab arg1 arg2 ... argN (You can pass multiple arguments).

If you enter only the command without arguments, it will include everyone in the channel and generate groups.

	•	The first argument is the size of each group.
 
	•	The second to N-th arguments are names of members to exclude.

For example, if you want groups of 4 and want to exclude Alex:

Type: /randbab 4 Alex

Our program will generate groups of 4, excluding Alex.

## _Flow_

```
Random_bab_generator/
├── .gitignore
├── README.md
├── build/
│   ├── CMakeLists.txt
│   └── Manifest.json
├── src/
│   ├── array.c
│   ├── main.c
│   ├── shuffle.c
│   ├── slack_api.c
│   └── slack_bot.py
└── inc/
    ├── array.h
    ├── shuffle.h
    ├── .slack_api.h
    └── .env
```

1. Run slack_bot.py — it waits for a command via socket communication with your Slack workspace.

2. When the command is received, the Python program passes the arguments to the C program.

3. The C program handles the grouping logic and prints the result.

4. slack_bot.py sends the result to the Slack workspace.

## _What is left?_

[ ] Make the slack bot into a real app

[ ] Improve fairness by considering more user info (e.g., gender)
