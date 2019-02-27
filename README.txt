GROUPE : ahmed mouhamaa, virgile langlet, jean-baptiste gaeng

Contient code c++ utilisant la base du code fournie en TP de sia1. Le program c++, myViewer, réalise une visualisation d'un fichier au format bvh.
Pour visualiser un autre fichier que celui par défaut, changer le PATH du fichier dans le code c++.
Faire attention à ce que la PATH du fichier bvh soit correct. Skinning non implémenté, visualisation du squelette avec GL_DRAW_LINES (une ligne par articulation).

Le code passe de frame en frame avec un timerEvent.
Pour changer la framerate changer la valeur dans starttimer(x), cela correspondra a une frame toutes les x secondes. (si on met un x trop petit, le
temps de calcul entre deux frames sera plus long, alors notre programme met un flag qui lui fait attendre la prochaine frame, évitant ainsi les data 
races proprement).

Nous avons enlevé le dossier contenant les mtbs pour que l'archive soit moins lourde

===================


il faut trimesh et aussi boost pour la compilation.
(qmake et make)


Contient aussi un début de solution pour l'animation par capteur. A partir des données du capteur, est fournit un script python qui les parse et obtient les
frames (sans faire le traitement des frames, qui n'est lui pas implanté, en effet il faudrait appliquer R^-1 du parent aux rotations du joint courant pour
obtenir les valeurs de rotations correctes pour le bvh).
Ce script python ajoute les frames (sans traitement) en parsant les données de mtb .txt (la encore il faut éditer proprement les PATHs pour que ça marche), et le
rajoute à la fin du skeleton_base.bvh (attention le script rajoute toujours en fin de fichier, il faut donc enlever les frames du squelette de base avant de le relancer).

Le squelette de base est basé sur la hiérarchie correspondante aux capteurs que l'on a, dont on avait la correspondance capteur-joint. Les offsets ont été faits a la main
en étalonnant avec l'outil de translation sous maya.

mtb_data contient l'information des capteurs. cette information a été reprise à un autre groupe, nos mesures de capteurs réalisé en début de projet n'étaient pas bonnes car nous n'avions pas mémorisé ce qu'il fallait pour pouvoir les réutiliser proprement. De plus nous n'avions que quelques capteurs. (ici 18 pour full body).
