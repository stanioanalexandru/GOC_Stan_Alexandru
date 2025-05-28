# Game of Life - Proiect PA

Acest proiect implementează o versiune extinsă a jocului Game of Life, folosind arbori binari, stive și grafuri pentru a simula evoluția celulelor și a analiza conexiunile lor.

## 🔧 Fișiere sursă

- `gameoflife.c` – fișierul principal cu logica pentru input, generarea arborelui de generații și rezolvarea cerințelor.
- `tas2_corectat.c` - versiunea ceruta a taskului 2, in care generatiile sunt aflate in functie de modificarile aflate in stiva, fara a construi o matrice de fiecare data
- `bonus.c` - taskul bonus, in care se "inverseaza" cerinta de la taskul 2
  
## 🧪 Cerințe implementate

- ✅ Task1: Construirea arborelui de generații (reguli standard și personalizate).
- ✅ Task2: Calculul stării unei celule pe baza stivei generațiilor.
- ✅ Task3: Arbori: aplicarea unei noi reguli a jocului, in care fiecare generatie se imparte in 2: una cu regula obisnuita,  si una cu regula cea noua
- ✅ Task4: Grafuri: Determinarea celui mai lung lanț Hamiltonian într-un graf generat de celulele vii.
- ✅ Bonus: reconstruirea matricei initiale pe baza modificărilor din generații (task bonus pentru task 2).

## 📥 Cum rulezi

1. Compilează codul:

```bash
gcc gameoflife.c -o t
./t ./InputData/data1.in ./Output/data1.out
```
2.Pentru a deschide checkerul
``` bash
./checker-linux-amd64 -i
```

## 🧠Scurta analiza a codului
--datele de intrare sunt sunt urmatoarea  forma:
1) t: numarul taskului
2) n,m: numarul de linii si coloane a matricei
3) k: numarul generatiei la care se ajunge
4) matricea efectiva
--fiecare task este aflat sub forma unui switch:
pentru task1 se alege t=1;
pentru task2 se alege t=2;
pentru task3 se alege t=3;
pentru task4 se alege t=4;

## 🚩Explicarea functiilor esentiale pentru fiecare task
### 🔹 Task 1 – Simularea jocului

Funcții relevante:
- `numar_vecini_vii` – calculează vecinii vii pentru o celulă
- `generare_generatii` – aplică regulile clasice ale jocului Life pentru `k` generații

### 🔹 Task 2 – Stivă cu modificări
--VARIANTA INITIALA--  
Funcții relevante:
- `push`, `printStack_inverted`, `deleteStack` – implementare stivă cu liste
- `creare_stiva` – creează stiva de modificări ale celulelor pentru fiecare generație
  In aceasta varianta am facut o apelare multipla a stivei, insa nu era o stiva de lista propriu zisa, stiva era folosita in contextul unei singure generatii, folosind matricea, iar dupa aceea resetata;

  ✅--VARIANTA FINALA--
-In aceasta varinata, prezenta in `task2_corectat.c`,  am format o stiva reala de liste, unde fiecarei celula ii era aflata starea parcurgand aceasta stiva, aflata sub forma structurii `Stiva2`  

  Functiile esentiale in aceasta varianta a taskului sunt:

  1.` is_alive() `- prezinta baza programului; apeleaza pentru fiecare grupare (l,c) stiva , modificandu-si starea (1 - in viata; 0 - moarta) pentru fiecare gasire in lista a celulei

  2.`vecini_vii_t2()` - numara vecinii vii pentru celule, folosind functia is_alive()

  3.`generare_lista()` - genereaza lista in functie de regulile jocului

  Pentru a avea o afisare corecta, inversam inainte stiva, iar dupa aceea aceasta se afiseaza
### 🔹 Task 3 – Arbore binar de generații  
Structuri adaugate:
- `NodeArbore` – structura pentru arbore
- 
Funcții relevante:

- `generare_lista()` - o functie esentiala; creeaza lista in fiecare nod aplicând reguli diferite pe ramuri(similara cu cea de la task2)
- `is_alive()`
- `creare_stiva_arbore()` – creează recursiv arborele de generații, folosind functia generare_lista
- `preorder()` – parcurgere în preordine pentru afișare

### 🔹 Task 4 – Graf și lanț Hamiltonian
Structuri adaugate:

`constructie_t4`

Funcții relevante:
- `constructie_t4` – reconstruieste matricea unui nod din arbore
- `construire_matrice_adiacenta` – creeaza graful celulelor vii
- `DFS` – determina componentele conexe
- `lant_hamiltonian` – determină lantul Hamiltonian maxim prin backtracking
- `construire_lant` – gestioneaza componentele si extrage rezultatul final

   
