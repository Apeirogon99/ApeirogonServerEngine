<div align="center">
  <!--Header-->
  <h1> Dark Crown : Return of the Lich </h1>
  <h2> 논 타겟팅 액션 MMORPG </h2>
</div>

<div>
  
  ### 📋구현 요약 : </br>
  #### System
  - [서버 처리 과정](#서버-처리-과정) : 네트워크 + 메인 로직 + 데이터베이스를 처리하는 서버 처리 과정
    - [메인로직 처리](#메인로직-처리) : 네트워크/DB 큐 처리, 월드 상태 업데이트
    - [네트워크 처리](#네트워크-처리) : IOCP모델을 통해 수신된 데이터 잡 큐에 저장
    - [데이터베이스 처리](#데이터베이스-처리) : ADO를 통해 MS-SQL 저장프로시저 실행

</div>

-------

<div align="center"> <h1> System </h1> </div>
<div align="center"> <h2> 서버 처리 과정 </h2> </div>

# 서버 처리 과정

### 설계 목표
적은 인원으로 많은 콘텐츠를 만들기 위한 서버 처리 과정 필요 </br>

### I/O 멀티 쓰레드, 메인 로직 싱글 쓰레드 구조
적은 인원으로 MMORPG를 만든 프로젝트의 강연을 보고 힌트를 얻어 제작하였습니다 </br>
동시 접속자는 제한되지만 디버깅 용이하고 개발 속도를 높일 수 있는 방법을 채택하였습니다. </br>

![image](https://github.com/user-attachments/assets/faa2c38d-3ff1-4e42-a2c6-4034443df417)

# 메인로직 처리

### 설계 목표
네트워크 / DB 큐에 쌓인 일을 처리하고 게임 월드 상태를 업데이트 하는 역할을 설계하고자 하였습니다. </br>

### 메인 로직 사이클
멀티 쓰레드에 비해 처리 능력은 다소 떨어질 수 있으나 많은 콘텐츠를 만들기 위하여 </br>
예측 가능하고 일관성있게 처리하여 개발 속도와 안정성을 높일 수 있도록 하기 위해 선택하였습니다. </br>

![image](https://github.com/user-attachments/assets/6fe5094f-ace0-44fb-bd40-016f0224fcc7)

<img width="1914" height="565" alt="image" src="https://github.com/user-attachments/assets/29cac0e5-75be-4c51-826c-8b9b6dcc1b25" />

### 처리 순서
#### 패킷 처리
클라이언트의 패킷과 Tick을 실행하며 잡 큐가 쌓이게 됩니다. </br>
잡 큐는 여러 쓰레드가 접근할 수 있기에 스핀락을 사용하였습니다. </br>
메인에서는 잡 큐에 현재 서버시간보다 낮은 잡을 처리합니다. </br>

<img width="639" height="177" alt="image" src="https://github.com/user-attachments/assets/31b73c41-2893-4042-bedb-241b51b680dd" />

#### 데이터베이스 처리
저장 프로시저를 실행한 결과가 완료 시 결과 큐로 저장하게 됩니다. </br>
메인에서는 task에 저장된 함수를 실행하여 결과를 처리합니다. </br>

<img width="635" height="335" alt="image" src="https://github.com/user-attachments/assets/3b8c5e46-bdf3-438f-8602-4e11c267f5b5" />

#### 월드 업데이트
고정된 DeltaTime값으로 월드 상태를 업데이트 합니다. </br>
Tick처리를 true로 설정한 오브젝트만 처리합니다. </br>

<img width="633" height="175" alt="image" src="https://github.com/user-attachments/assets/27c162ff-801c-4d7a-99c7-12f240ca7a20" />

#### 클라이언트에게 보내기
로직을 처리하며 업데이트된 패킷이 쌓여있다면 전송합니다 </br>

<img width="638" height="216" alt="image" src="https://github.com/user-attachments/assets/9569d86b-ab74-49d7-9870-91431959b5ce" />

# 네트워크 처리

### 설계 목표
클라이언트 패킷을 수신/송신하며, 수신된 데이터를 잡 큐에 전달하는 역할을 설계하고자 하였습니다. </br>

### IOCP 모델
MMORPG는 수천 명의 플레이어가 동시에 접속하여 실시간으로 상호작용이 발생하게 됩니다. </br>
그래서 IOCP의 확장성과 비동기 I/O 그리고 설계한 구조와의 조화에 대한 이점이 있기에 선택하였습니다. </br>

![image](https://github.com/user-attachments/assets/6056f30d-583c-46d5-81fd-762d73ad453e)

### 잡 큐
#### 모든 오브젝트에 잡 큐 문제
강의를 보며 만든 잡 큐는 클래스에 정의된 함수를 실행 하였고  </br>
World에는 관련성 없는 함수가 늘어나며 관리가 힘들었습니다.  </br>

그래서 모든 오브젝트에 잡큐를 넣으면 되지 않을까 생각했습니다.  </br>
하지만 동기화와 메모리 오버헤드면에서 아쉬운 처리였습니다.  </br>

<img width="851" height="254" alt="image" src="https://github.com/user-attachments/assets/4d79d939-26fc-4b39-acdd-e54c5603e9ab" />

#### 월드와 오브젝트 Id에 따른 잡 큐 처리 개선
다시 작성하는 것보다는 최대한 이용해보자고 생각하였습니다.  </br>
우선 동기화 문제와 메모리를 효율적으로 처리하기 위해서는  </br>
World(메인, 던전)에 잡 큐를 넣는 것은 맞다고 생각하였습니다. </br>

객체에 대한 실행은 객체에 유니크한 값이 있기에 ID로 검증하여 </br>
기존 PushTask를 이용하여 처리하는 아이디어를 생각하였습니다. </br>

다음과 같이 개선하여 많은 변경 없이 코드 작성이 가능합니다. </br>
직접 변경한것은 아니지만 동일한 로직을 만들어 확인하였습니다. </br>

<img width="850" height="308" alt="image" src="https://github.com/user-attachments/assets/3822c241-7b48-46bd-a346-998ffabd6738" />

# 데이터베이스 처리

### 설계 목표
데이터베이스를 처리하기 위해 쿼리를 실행하고, 결과를 큐에 넣는 역할을 설계하고자 하였습니다 </br>

### ADO를 래핑하여 저장 프로시저 처리
프라우드넷 엔진으로 ADO를 사용했던 경험으로 ADO를 래핑하여 저장 프로시저를 사용하기 쉽게 제작하였습니다. </br>
경험도 중요한 요소였습니다만, 개발 생산성과 검증된 기술이기에 선택하였습니다. </br>

<img width="1919" height="545" alt="image" src="https://github.com/user-attachments/assets/ef5af4e4-87ad-48aa-9ce6-edf878f7929b" />

### 저장 프로시저
직접 쿼리를 작성하는 것은 디버깅이 수월하지만 </br>
저장 프로시저의 데이터베이스에서 미리 컴파일된 실행 계획을 </br>
재사용하여 성능을 높이기 위해 사용하였습니다. </br>

<img width="803" height="464" alt="image" src="https://github.com/user-attachments/assets/5b5af82c-0154-4d1f-9145-8fdb4b984356" />

### ADO Task
저장 프로시저를 작성하고 비동기로 실행시키는 동안 </br>
connection, command, recordset은 존재해야하기 때문에 </br>
결과가 나와 처리될 때 까지 유지할 수 있도록 하였습니다. </br>

Task를 끝내고 Release를 할 떄에는 Connection을 먼저 끊으면 Recordset이 바로 닫히게 됩니다 </br>
그렇기에 다음과 같이 순서를 지켜서 Close와 Release를 하였습니다 </br>
<img width="183" height="258" alt="image" src="https://github.com/user-attachments/assets/5abd47ae-6509-422d-8cdd-a22ab90455ec" />


### 메인 로직에서 쿼리 결과 처리
메인 로직에서 데이터베이스 쿼리 결과를 처리하기 위해 </br>
함수를 실행하여 command에 저장된 결과 및 아웃풋을 확인하고 </br>
recordset으로 데이터 탐색이 가능하도록 만들었습니다. </br>

<img width="800" height="466" alt="image" src="https://github.com/user-attachments/assets/bda14b9f-28bc-4bdd-98f5-f43aba5d3bd0" />

### Execute 처리
저장 프로시저를 어떤 방식으로 실행시킬지 설정 할 수 있습니다. </br>
문서에 있는 내용을 보고 조합하였으며 다음과 같이 3가지의 옵션이 존재합니다. </br>
adAsyncExecute같은 경우에는 adAsyncFetchNonBlocking있지만 취지는 전자가 더 맞다고 생각하였습니다 </br>

<img width="613" height="266" alt="image" src="https://github.com/user-attachments/assets/acad5cc8-faaa-4e99-a7e8-6ae64a2decdb" />

비동기로 할경우 다음과 같이 executing하다가 complete하는 것을 볼 수 있습니다 </br>
<img width="412" height="68" alt="image" src="https://github.com/user-attachments/assets/dd7bc69e-5be7-4000-b337-5180a69d0a7b" />

### 타입 처리
타입의 경우 어떻게 해야할까 고민하다가 다음과 같이 정의하였습니다 </br>
이유는 SQL SERVER <-> C++ SERVER의 타입이  동일해야 하기 떄문입니다 </br>

<img width="682" height="459" alt="image" src="https://github.com/user-attachments/assets/058e42cf-1401-4031-a8b2-c79af24cdecb" /> </br>

없는 경우 따로 작성하였습니다 </br>
<img width="380" height="169" alt="image" src="https://github.com/user-attachments/assets/e47d700f-d95c-4c65-a80b-2abd2032e724" /> </br>

이를 이용해서 다음과 같은 작업이 가능하였습니다  </br>
Input값을 받아와서 해당하는 타입으로 자동 변경 되도록 하였습니다,  </br>
(Output의 경우 직접 정의하도록 하였습니다 = 이게 더 편하다고 생각하였습니다)  </br>
<img width="481" height="86" alt="image" src="https://github.com/user-attachments/assets/8491fa99-0c4d-425e-b77d-004c9d27df05" /> </br>

