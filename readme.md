Projet FTCPP : présentation d'un outil d'analyse de couverture de code d'un projet de gestion de connexion serveur FTP en C++

Membres du groupe: Djaouel Jenna-Louisa, Julien Audrey, Aissaoui Mohamed El Salah

# FTPGestion - Prérequis pour le projet

Pour pouvoir travailler sur ce projet, voici ce que vous devez installer et configurer :

## Logiciels requis

- **MinGW-w64** (version recommandée : 14.2.0)
  - Assurez-vous que `mingw32-make` est accessible dans le `PATH` de votre système.
  - Le compilateur doit supporter `g++` avec au minimum la norme C++14.
  - `gcov` est fourni avec MinGW-w64 et sera utilisé pour la couverture de code.

- **CMake**
  - Version 3.20 ou supérieure recommandée.
  - Doit être accessible via la commande `cmake` dans le `PATH`.

- **Python 3.13**
  - Nécessaire pour lancer certains scripts de build et de couverture.

- **Paquets Python**
  ```bash
  pip install cmake


## Execution de la couverture

- **Couverture**
   - Pour effectué la couverture de code il vous suffit simplement d'éxécuter le `Setup.bat`