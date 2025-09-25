# 랜덤밥 생성기

<p align="center">
  <img src="./img/randombab.png" alt="Alt Text" width="150">
</p>

**프로젝트 동기** :

매주 목요일에 있는 랜덤밥 그룹을 조금 더 형평성 있게!  
매니저님들 손 안 아프게 하기 위한 자동화 프로그램

## 실행 방법

### 사전 준비

다음 패키지가 설치되어 있어야 합니다:

- jansson  
- curl  
- cmake  
- virtualenv  

⚠️ Slack에서 발급받은 **app token**과 **bot token**이 필요합니다.  
`.env` 파일에서 본인 토큰으로 변경해야 합니다. ⚠️  

⚠️ Slack 봇은 반드시 **SOCKET MODE**로 실행되어야 합니다. ⚠️

### MacOS

```bash
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

→ 이제 Slack 워크스페이스에서 봇이 커스텀 명령어를 기다리고 있습니다!  
(예: `/randbab`)

### Windows

```
찬우 반장님께 물어보세요!
```

### Linux

```
아직 시도해 보지 않았습니다 :-)
```

---

## 사용 방법

기본 명령어는 다음과 같습니다:

```
/randbab arg1 arg2 ... argN
```

- 인자 없이 `/randbab`만 입력하면, 채널에 있는 모든 멤버를 포함해 그룹을 생성합니다.  
- **첫 번째 인자**: 그룹의 크기  
- **두 번째 ~ N번째 인자**: 제외할 멤버 이름  

예시:  
4명씩 그룹을 만들고 `Alex`를 제외하려면:

```
/randbab 4 Alex
```

라고 입력하면 됩니다.

---

## 폴더 구조 & 흐름

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

1. `slack_bot.py` 실행 → Slack 워크스페이스와 소켓 통신으로 명령어 대기  
2. 명령어 수신 시 Python 프로그램이 인자를 C 프로그램에 전달  
3. C 프로그램에서 그룹 로직 실행 후 결과 출력  
4. Python 프로그램이 결과를 Slack 워크스페이스에 전송  

---

## 앞으로 할 일

- [ ] Slack 봇을 실제 앱으로 만들기  
- [ ] 성별 등 더 많은 유저 정보를 고려하여 형평성 개선하기  
