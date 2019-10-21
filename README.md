# TP Mesh Computation M2 ID3D

- Ali 	Zahidi  p1713059
- Robin Donnay	p1510329
## Tâches effectuées
    - Toutes les fonctions et prédicats (ainsi que les ajouts pour Voronoi) ont été implémenté.
## Remarques
- Flip implementé  : 
    - Le bouton "Random Flip" feras automatiquement un flip sur les faces 9 et 10 (précoder pour la maillage)
    - On peut aussi choisir l'index de deux faces voisine et les flipper grâce au bouton "flip", dans le groupe de widget "Flipping"
- Split implementé :
    - Faire une insertion naive grâce au bouton "Naive insert" dans le groupe "Insertion" permet de visualiser le split (Des valeurs ont déjà été précodé pour visualiser sur le maillage)
- Rendre un maillage de Delaunay
    - Dans l'onglet "Delaunay", le bouton "Delaunize" permet de rendre un maille de Delaunay
    - Utilise un predicat EstDeDelaunayLocal appellé "isLocallyOfDelaunay()" dans Mesh.cpp
- Visualisation de Voronoi
    - Dans l'onglet "Voronoi", il suffit de cocher la case "Show Voronoi" et d'appuyer sur "Compute Voronoi"
