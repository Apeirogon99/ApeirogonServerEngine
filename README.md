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
![image](https://github.com/user-attachments/assets/faa2c38d-3ff1-4e42-a2c6-4034443df417)

# 메인로직 처리
![image](https://github.com/user-attachments/assets/6fe5094f-ace0-44fb-bd40-016f0224fcc7)

# 네트워크 처리
![image](https://github.com/user-attachments/assets/6056f30d-583c-46d5-81fd-762d73ad453e)

# 데이터베이스 처리
![image](https://github.com/user-attachments/assets/d3384ab5-84b1-4e47-a490-1cbbb07ebaac)

