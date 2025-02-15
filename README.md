# 🧠 Philosophers

## 📝 Présentation

**Philosophers** est un projet d’**algorithmie concurrente** permettant d’explorer les **threads**, les **mutexes**, et la gestion de la synchronisation entre processus.  
Le but est d’implémenter une **simulation** où plusieurs philosophes sont assis autour d’une table, partageant des fourchettes et alternant entre trois états : **manger, penser et dormir**.

Le projet met l’accent sur la **prévention des interblocages (deadlocks)** et la gestion efficace des **ressources partagées**.

---

## 🛠️ Fonctionnalités

- **Création de plusieurs threads (philosophes) exécutant simultanément leurs tâches**.
- **Utilisation de mutexes pour synchroniser l'accès aux fourchettes**.
- **Gestion du cycle de vie des philosophes** :
  - **Manger** : Un philosophe doit prendre **deux fourchettes** pour manger.
  - **Dormir** : Après avoir mangé, il dort un certain temps.
  - **Penser** : Lorsqu’il se réveille, il commence à penser.
- **Détection et gestion de la mort d’un philosophe** si `time_to_die` est dépassé.
- **Affichage des logs** :
  - `timestamp_in_ms X has taken a fork`
  - `timestamp_in_ms X is eating`
  - `timestamp_in_ms X is sleeping`
  - `timestamp_in_ms X is thinking`
  - `timestamp_in_ms X died`
- **Aucune condition de concurrence (data race) n'est tolérée**.

---

## 📌 Technologies Utilisées

- **C (Norminette respectée)**  
- **Threads POSIX (`pthread_create`, `pthread_join`)**  
- **Mutexes (`pthread_mutex_init`, `pthread_mutex_lock`, `pthread_mutex_unlock`)**  
- **Gestion du temps (`usleep`, `gettimeofday`)**  
- **Synchronisation et évitement des interblocages**  

---

## 🏗️ Structure du Projet

📂 philo  
┣ 📂 .vscode  
┣ 📂 headers  
┣ 📜 philosophers.h  
┣ 📂 srcs  
┣ 📜 brain_life.c  
┣ 📜 init.c  
┣ 📜 main.c  
┣ 📜 parsing.c  
┣ 📜 philo_life.c  
┣ 📜 utils.c  
┗ 📜 Makefile  

---

## 🔥 Difficultés Rencontrées

- **Éviter les deadlocks** : Gestion de l’ordre de prise des fourchettes pour empêcher une situation d’attente circulaire.  
- **Gestion efficace des mutexes** : Assurer une exécution fluide sans blocage intempestif.  
- **Respect des délais (`time_to_die`, `time_to_eat`, `time_to_sleep`)** pour garantir une simulation correcte.  
- **Synchronisation et communication inter-threads** pour éviter les conditions de concurrence.  
- **Optimisation des accès mémoire** pour limiter l’impact des locks/délocks fréquents.  

---

## 🏗️ Mise en place

1. **Cloner le dépôt** :  
   ```bash
   git clone https://github.com/ton-repo/philosophers.git
   cd philosophers
2. **Compiler le projet** :
   ```bash
   make
3. **Lancer la simulation avec plusieurs philosophes** :
   ```bash
   ./philo 5 800 200 200
- 5 : Nombre de philosophes.
- 800 : time_to_die (ms).
- 200 : time_to_eat (ms).
- 200 : time_to_sleep (ms).
4. **Lancer avec un nombre minimum de repas par philosophe** :
   ```bash
   ./philo 5 800 200 200 5
5. **Nettoyer les fichiers compilés** :
   ```bash
   make clean
   make fclean

---

## 👨‍💻 Équipe  

👤 Grégoire Chamorel (Gchamore)  

---

## 📜 Projet réalisé dans le cadre du cursus 42.  

Tu peux bien sûr modifier ce README selon tes besoins, ajouter plus de détails sur ton approche et des instructions d’installation précises. 🚀  
