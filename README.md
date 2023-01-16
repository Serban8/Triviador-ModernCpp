# Triviador-ProjectModernCpp

Video presentation:

https://we.tl/t-erTWpj5JDx

https://youtu.be/y8eD95iPXzg

Useful links for question databases:
- OpenTDB: https://opentdb.com
- The Trivia Api: https://the-trivia-api.com

Useful links for libraries and helper headers:
- cpr (curl): https://github.com/libcpr/cpr
- sqlite-orm: https://github.com/fnc12/sqlite_orm
- nlohmann - json parser: https://github.com/nlohmann/json

# ER Diagram for Data Base

![Triviador_DataBase_ER_Diagram](https://user-images.githubusercontent.com/77741850/212502683-af98e51e-7491-4590-8024-082d3b6ce398.jpg)

Some snapshots regarding app profiling(CPU usage and memory usage):
- memory usage when showing the waiting room list

![image](https://user-images.githubusercontent.com/77741850/212574835-5a7b7fa5-2e93-497d-9f3e-7c8ddfbb09f7.png)
![image2](https://user-images.githubusercontent.com/77741850/212574838-3d75e8c2-cf78-40a0-87b2-0c4c30aed3b5.png)

- CPU usage when retrieving questions from database when a game is launched:
![image4](https://user-images.githubusercontent.com/77741850/212574977-5892f213-dbad-4435-8051-dca52ef177a4.png)


- CPU usage when populating the database with questions for the first time:
![image3](https://user-images.githubusercontent.com/77741850/212574944-8135de3b-40b4-4737-a3ab-885b1fb334f6.png)
