# 임베디드 시스템 설계 메인프로젝트<br/>[221113 ~ 221215]
인공지능 기반 연기, 온도 감지 쿨링팬

## 팀원
김남주 박수영 안중보

## 목적
- 컴퓨터의 성능이 점점 좋아지면서 쿨링 성능의 중요성 또한 커졌다. 좋아지는 컴퓨터의 성능에 맞춰 스마트한 쿨링 시스템을 만들었다.
- 컴퓨터 본체의 과열을 방지하기 위해 연기와 온도를 실시간으로 감지하여 식혀줄 수 있는 쿨링 시스템을 생각했다. 사람이 직접 조작할 필요 없이 자동으로 작동하는 프로그램을 개발하였다.
- 컴퓨터 외에 다른 데이터센터 등 과열과 화재에 취약한 곳에서 사용하여 화재가 나기전에 연기를 감지해 피해를 최소화할 수 있다.

## 결과물 형태
<div align="center">
  
![순서도](https://github.com/zoo3323/AI_cooling_fan/assets/95582592/33a38120-7624-4b46-beba-50c7d6f62d15)
</div>

## 사용한 모델
roboflow 사전 훈련 모델 ([Smoke100 Computer Vision Project](https://universe.roboflow.com/smoke-detection/smoke100-uwe4t))을 사용하여 연기탐지를 하였다.

![탐지](https://github.com/zoo3323/AI_cooling_fan/assets/95582592/01dcdd7a-e2c7-48af-bcec-3bc91a49c212)

## 작품 사양
1. 하드웨어 구성
- 가변저항을 통해 dc모터의 속도를 조절할 수 있다.

![가변저항](https://github.com/zoo3323/AI_cooling_fan/assets/95582592/281ccae0-a5fa-4977-98d8-db6eb4f2a8c4)


- 온도센서의 값에 따라 led, 모터, 피에조부저가 작동된다.

![온도센서](https://github.com/zoo3323/AI_cooling_fan/assets/95582592/894602d9-b286-41bb-9a3d-d37e576f8906)


2. 소프트웨어 구성
- 연기 탐지 모델을 사용하여 여러 하드웨어를 작동시킨다.
  
![연기탐지모델](https://github.com/zoo3323/AI_cooling_fan/assets/95582592/0de4a742-283e-49db-9cd5-2279179aa76a)
