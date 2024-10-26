

# Genesis OS

**Genesis OS** est un système d'exploitation développé en **C** dans le but d'explorer les principes de base et avancés des systèmes d'exploitation. Ce projet propose des fonctionnalités de gestion des processus, de protection du kernel, et inclut un shell interactif pour illustrer différents concepts.

---
```
       :                       ..,,..    ...,,.. 
      ,%,                .. ,#########::#########:, 
      :#%%,           ,,:',####%%%%%%##:`::%%%%####, 
     ,##%%%%,      ,##%% ,##%%%:::::''%' `::::%%####, 
     %###%%;;,   ,###%%:,##%%:::''    '  . .`:::%%###, 
    :####%%;;:: ,##%:' ,#%::''   .,,,..    . .`::%%%##, 
    %####%;;::,##%:' ,##%''  ,%%########%     . `:::%%##, 
    ######:::,##%:',####:  ,##%%:''     `%%,     .`::%%##, 
    :#####%:'##%:',#####' ,###%' ,%%%%,%%,'%,     . ::%%###,,.. 
     #####%:,#%:'#######  %%:'%  %'  `%% %% %%,.     '::%%#######, 
     `####%,#%:',####### ::' %   ' ,%%%%%%, ::%%.    . '::%%###### 
      `###'##%: ######## ,.   %%  %%,   ':: `:%%%  :  . .:::%%###' 
      ,,::,###  %%%%%### ::  % %% '%%%,.::: .:%%%   #.  . ::%%%#' 
,,,:::%%##:;#   `%%%%%## :% ,%, %   ':%%:'  #%%%' ,.:##.  ::%#' 
::%%#####% %%:::  :::%%% `%%,'%%     ..,,%####' :%# `::##, '' 
###%%::'###%::: .   `:::, `::,,%%%######%%'',::%##' ,:::%## 
''''   ,####%:::. .  `::%,     '':%%::' .,::%%%#'   :::%%%##, 
      :#%%'##%:::.  . . "%::,,.. ..,,,,::%%%###'  ,:%%%%####' 
     ,###%%'###%:::: . . `::::::::::%%%#####'   ,::%####:' 
     %###%%;'###%::::.   .`::%%%%%%%#####:'  ,,::%%##:' 
     ####%;:;'####%:::::.   `:%######::'  ,,:::%%### 
     %####;:;'######%%::::.           ,::::%%%####' 
     `####%;:'`#########%%:::....,,:::%%%#######' 
        ;#####;;'..;;:::#########::%%#########:' 
                       ~~~~``````''''~~~ 
```
---

## Aperçu des Fonctionnalités

- **Gestion des Processus** : Genesis OS permet de gérer les processus dans divers états (actif, endormi, zombie, en attente de terminaison, etc.) avec un contrôle des priorités et de l’ordonnancement.
- **Modes Kernel et Utilisateur** : Un mode kernel protégé et un mode utilisateur, chacun avec des niveaux d'accès spécifiques pour assurer la stabilité et la sécurité du système.
- **Synchronisation et Sémaphores** : Implémentation de sémaphores pour un accès structuré et sécurisé aux ressources partagées.
- **Fonctions Système** : Genesis OS supporte des fonctionnalités telles que la gestion de signaux, une horloge système synchronisée avec les processus et des commandes intégrées pour la manipulation des processus.

## Shell Interactif avec Programmes

Le shell intégré de Genesis OS offre une expérience interactive avec divers programmes pour illustrer les capacités du système :

- **Piano Virtuel** : Un instrument numérique jouable directement depuis le terminal.
- **Jeu du Pendu** : Un jeu classique pour une pause récréative.
- **Lecteur de Musique** : Accès à une bibliothèque musicale avec lecture directe.
- **Commandes de Gestion des Processus** : Pour créer, suspendre et terminer des processus, interagir avec les sémaphores, et plus encore.

## Points d'Intérêt

Genesis OS a été conçu comme un terrain d'apprentissage et d'exploration des concepts des systèmes d'exploitation :

- **Protection du Kernel** : Sécurisation des fonctions critiques et isolation des accès entre les processus en mode utilisateur et kernel.
- **État des Processus** : Exploration de plusieurs états pour chaque processus, offrant une gestion détaillée.
- **Exploration des Concepts d'OS** : Chaque fonctionnalité illustre les fondements théoriques des systèmes d'exploitation, appliqués dans un environnement en C.

## Compilation et Exécution

### Pré-requis

- **Compilateur C**
- **QEMU** (optionnel, pour un environnement virtualisé)

### Compilation

Clonez le dépôt, puis compilez avec :

```bash
git clone https://github.com/FelinaeBlanc/Kernel_GenesisOS.git
cd genesis_os
make
