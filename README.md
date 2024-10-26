

# Genesis OS

**Genesis OS** est un système d'exploitation complet développé from scratch en **C**, conçu pour explorer et illustrer les principes fondamentaux et avancés d'un OS moderne. Ce projet s'étend bien au-delà d'un OS de base, intégrant des fonctionnalités complexes dans la gestion des processus, la protection du kernel, et une interface utilisateur interactive via son shell intégré.

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

- **Gestion Avancée des Processus** : Genesis OS permet de gérer les processus à divers états (actif, endormi, zombie, en attente de terminaison, etc.) avec un contrôle fin des priorités et de l’ordonnancement.
- **Modes Kernel et Utilisateur** : Un mode Kernel protégé et un mode utilisateur, chacun avec des niveaux d'accès spécifiques, assurant la stabilité et la sécurité du système.
- **Synchronisation et Sémaphores** : Implémentation de sémaphores pour un accès structuré et sécurisé aux ressources partagées.
- **Fonctions Système Riches** : Genesis OS supporte des fonctionnalités telles que la gestion de signaux, une horloge système synchronisée avec les processus, et des commandes intégrées pour la manipulation des processus.

## Shell Interactif avec Programmes

Le shell intégré de **Genesis OS** offre une expérience interactive avec divers programmes pour démontrer les capacités du système :

- **Piano Virtuel** : Un instrument numérique jouable directement depuis le terminal.
- **Jeu du Pendu** : Un jeu classique pour une pause ludique.
- **Lecteur de musique** : Accès à une bibliothèque musicale avec lecture directe.
- **Commandes de Gestion des Processus** : Pour créer, suspendre, et terminer des processus, interagir avec les sémaphores, et bien plus.

## Points Forts

**Genesis OS** a été conçu pour être un terrain d'apprentissage et d'exploration des concepts des systèmes d'exploitation :

- **Protection Avancée du Kernel** : Sécurisation des fonctions critiques et une isolation des accès entre les processus en mode utilisateur et kernel.
- **État des Processus** : Genesis OS explore des états multiples pour chaque processus, offrant une granularité unique dans leur gestion.
- **Exploration des Concepts d'OS** : Chaque fonctionnalité est une illustration des fondements théoriques des OS, appliquée dans un environnement en C.

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
