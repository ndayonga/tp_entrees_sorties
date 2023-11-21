# TP_Entrees_Sorties

## Compilation 

Faire simplement `make`.

## Tests :

Recopie de fichier :
```
echo "Je suis un fichier de test\nAvec plusieurs lignes\n;)" > toto1
./test toto1 toto2
```

Test simple et fourni de fprintf et fscanf
```
./test-simple toto3
./test-format totoecr toformat.txt
```

Test avec des éléments plus grand que le buffer
```
./test-bigread to1 to2
./test-bigwrite to3
```

Test fourni random et comparaison
```
./test-rand
```

## Performances

Test de rapidité de write et read sur deux exemples
```
./eval-write
./eval-read
```