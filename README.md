# TP Mesh Computation M2 ID3D

- Ali 	Zahidi  p1713059
- Robin Donnay	p1510329

## Tâches effectuées
    - Toutes les fonctions et prédicats (ainsi que les ajouts pour Voronoi) ont été implémenté.

## Fonctionnalités
- Flip implementé  : 
    - Le bouton "Random Flip" fera automatiquement un flip sur les faces 9 et 10 (précoder pour la maillage)
    - On peut aussi choisir l'index de deux faces voisine et les flipper grâce au bouton "flip", dans le groupe de widget "Flipping" (prudence avec le flip de faces infinis)
- Split implementé :
    - Faire une insertion naive grâce au bouton "Naive insert" dans le groupe "Insertion" permet de visualiser le split (Des valeurs ont déjà été précodé pour visualiser sur le maillage)
- Rendre un maillage de Delaunay
    - Dans l'onglet "Delaunay", le bouton "Delaunize" permet de rendre un maille de Delaunay
    - Utilise un predicat EstDeDelaunayLocal appellé "isLocallyOfDelaunay()" dans Mesh.cpp
- Visualisation de Voronoi
    - Dans l'onglet "Voronoi", il suffit de cocher la case "Show Voronoi" et d'appuyer sur "Compute Voronoi"

## Amélioration et corections faites après la mini-démonstration
- L'insertion naive gère l'insertion de points ne respectant pas l'enveloppe convexe (flip de face infinis pour recréer l'enveloppe convexe) 

## Tests intéressants et représentatifs 
#### Pour bien voir l'impact de chaque test, il est conseillé de relancer l'application a chaque fois  
- L'insertion naïve ou de Delaunay du point : x=0.5, y=0.5, z=0 montre l'insertion d'un point à l'interieur du maillage. Cela permet de voir aussi la bonne implémentation de la fonction de split d'un triangle (tout en utilisant les prédicats d'appartenance à un triangle et d'orientation).
- L'insertion naïve ou de Delaunay du point : x=3, y=3, z=0 montre l'insertion d'un point hors de l'enveloppe convexe. On voit bien que l'insertion naïve modifiel'enveloppe pour garder une enveloppe convexe 

