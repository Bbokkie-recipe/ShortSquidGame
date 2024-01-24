
# Squidgame_Test

Developed with Unreal Engine 5   
유튜브 링크 : https://youtu.be/8r4_z2gNfqc?feature=shared   

언리얼 네트워크 멀티플레이어 2인 팀프로젝트 : 서바이벌 러닝게임    
개발 기간: 2024.01.09~2024.01.22(2주)   
개발 엔진: Unreal 5.3.2   

🧑🏻‍💻 Team   
- memberA : Game State 시스템 및 UI/사운드 동기화, 맵   
- memberB : 플레이어 애니메이션, Doll 타이머, stage Actors(Line, Item)   

🎞️ 게임 소개      
![image](https://github.com/Bbokkie-recipe/ShortSquidGame/assets/93479286/98222e17-53ff-497d-882c-0e65783f68bc)
넷플릭스 드라마 '오징어게임' 오마주.   
‘무궁화 꽃이 피었습니다’ 게임 룰에 따라 노래가 흐를 때만 이동하여 Final Line에 도달하면 생존

🛠️ 주요 기능   
- 세션 create, find, join 기능
  ![image](https://github.com/Bbokkie-recipe/ShortSquidGame/assets/93479286/863af414-39f6-44fe-bdf5-d3151a86d924)   

- game Ready등의 player state 실시간 동기화 -> UI 출력   
  (하단 캡처: 리슨 서버 호스트 start와 참가자 ready)   
![image](https://github.com/Bbokkie-recipe/ShortSquidGame/assets/93479286/b1d02e9a-4100-407e-8ea4-2b142276f43e)   

- 플레이어 애니메이션 동기화   
![image](https://github.com/Bbokkie-recipe/hnm-clone/assets/93479286/0762b3b3-820a-489c-9d91-46e8b0ff9717)   

- Final Line 통과 플레이어목록 업데이트에 따른 결과 UI
![image](https://github.com/Bbokkie-recipe/hnm-clone/assets/93479286/b9d6fbe0-7dab-4830-8a2a-18327d2ad0a2)   

- Doll Sound 동기화    
https://github.com/Bbokkie-recipe/ShortSquidGame/commit/aa7f69b9eeea6957270d43c8b9e40401ccb9eacc

- Doll 타이머 State에 맞춘 Player 움직임 Die 판정   
https://github.com/Bbokkie-recipe/ShortSquidGame/commit/b36acad5cd96827e67d8a5edd39f02130c71e6a3
![image](https://github.com/Bbokkie-recipe/hnm-clone/assets/93479286/24530b0d-03de-4c99-9eb9-b606c11b894d)
![image](https://github.com/Bbokkie-recipe/hnm-clone/assets/93479286/a1044d35-89a1-44e2-a07b-b7c626061ffb)
