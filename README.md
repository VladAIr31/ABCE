# Analiza Performanței Structurilor de Date: Arbori Red-Black, Skip Lists și Treaps

Acest proiect analizează și compară performanța a trei structuri de date avansate: Arbori Red-Black, Skip Lists și Treaps. Testarea se concentrează pe eficiența operațiilor fundamentale într-o varietate de scenarii.

## Definiții Generale

### Arbori Red-Black (Red-Black Trees)
Un arbore Red-Black este un tip de arbore binar de căutare auto-echilibrant, o structură de date notabilă pentru stocarea și recuperarea rapidă a informațiilor ordonate. Nodurile într-un arbore Red-Black conțin un bit suplimentar de "culoare", adesea desenat ca roșu și negru, care ajută la asigurarea faptului că arborele este întotdeauna aproximativ echilibrat. Proprietățile specifice ale culorilor (de exemplu, rădăcina este neagră, niciun nod roșu nu are un părinte roșu, toate căile de la un nod la descendenții săi frunză conțin același număr de noduri negre) garantează că cea mai lungă cale de la rădăcină la o frunză nu este mai mult decât dublul celei mai scurte căi. Acest lucru asigură o complexitate logaritmică O(log n) pentru operațiile de căutare, inserare și ștergere în cel mai rău caz.

### Skip Lists
O Skip List este o structură de date probabilistică ce permite căutarea, inserarea și ștergerea eficientă a elementelor într-o listă sortată. Este construită în straturi, unde stratul inferior este o listă înlănțuită ordinară. Fiecare strat superior conține un subșir al elementelor din stratul inferior, acționând ca o "bandă rapidă" pentru a sări peste elemente. Nodurile sunt promovate la straturi superioare pe baza unui proces probabilistic (de exemplu, aruncarea unei monede). Skip Lists oferă o complexitate medie de O(log n) pentru operațiile principale, fiind o alternativă mai simplu de implementat la arborii echilibrați.

### Treaps
Un Treap (sau arbore cartezian) este o structură de date care combină proprietățile unui arbore binar de căutare (BST) și ale unui heap binar. Fiecare nod dintr-un treap stochează o cheie și o prioritate (generată de obicei aleatoriu). În ceea ce privește cheile, structura satisface proprietatea BST (cheile din subarborele stâng sunt mai mici, iar cele din subarborele drept sunt mai mari). În ceea ce privește prioritățile, structura satisface proprietatea de heap (prioritatea oricărui nod este mai mare sau egală cu prioritățile copiilor săi). Datorită priorităților aleatorii, un treap are o înălțime așteptată logaritmică, ducând la o complexitate așteptată de O(log n) pentru operațiile de bază.

## Operații Testate

Setul de teste evaluează următoarele operații pe fiecare structură de date:

1.  **Inserare (Operația 1):** Inserează numărul `X` în mulțime.
2.  **Ștergere (Operația 2):** Șterge numărul `X` din mulțime (dacă acesta există).
3.  **Căutare (Operația 3):** Afișează `1` dacă numărul `X` este în mulțime, altfel afișează `0`.
4.  **Predecesor (Operația 4):** Afișează cel mai mare număr `Y`, mai mic sau egal cu `X`.
5.  **Succesor (Operația 5):** Afișează cel mai mic număr `Y`, mai mare sau egal cu `X`.
6.  **Interval (Operația 6):** Afișează în ordine sortată toate numerele `Z`, unde `X ≤ Z ≤ Y`.

## Descrierea Cazurilor de Test

Fișierele de test (`*.in`) sunt denumite pentru a reflecta distribuția operațiilor:

* **`1.in`**: Accent pe operația 1 (inserare). Operația 1 reprezintă 65% din totalul operațiilor, iar restul de 5 operații au fiecare un procent de 7%.
* **`2.in`**: Accent pe operația 2 (ștergere). Similar, operația 2 are un procent de 65%, iar restul câte 7%.
* **`3.in`**: Accent pe operația 3 (căutare).
* **`4.in`**: Accent pe operația 4 (predecesor).
* **`5.in`**: Accent pe operația 5 (succesor).
* **`6.in`**: Accent pe operația 6 (interval).
* **`eq.in`**: Operațiile sunt distribuite aproximativ egal ca număr.

## Rezultate

Mai jos sunt timpii de execuție (în milisecunde) obținuți pentru fiecare structură de date și fiecare caz de test, conform fișierului `results.txt`:

### Red-Black Tree
* Test: `1.in`, Timp: 764376 ms
* Test: `2.in`, Timp: 54946 ms
* Test: `3.in`, Timp: 55843 ms
* Test: `4.in`, Timp: 55768 ms
* Test: `5.in`, Timp: 55913 ms
* Test: `6.in`, Timp: 520200 ms
* Test: `eq.in`, Timp: 342273 ms

### Skip List
* Test: `1.in`, Timp: 1471004 ms
* Test: `2.in`, Timp: 66251 ms
* Test: `3.in`, Timp: 67200 ms
* Test: `4.in`, Timp: 68728 ms
* Test: `5.in`, Timp: 67080 ms
* Test: `6.in`, Timp: 644523 ms
* Test: `eq.in`, Timp: 551456 ms

### Treaps
* Test: `1.in`, Timp: 741126 ms
* Test: `2.in`, Timp: 60895 ms
* Test: `3.in`, Timp: 63235 ms
* Test: `4.in`, Timp: 62872 ms
* Test: `5.in`, Timp: 62566 ms
* Test: `6.in`, Timp: 591203 ms
* Test: `eq.in`, Timp: 360032 ms

## Analiza Rezultatelor (Exemplu General)

* **Operații de Inserare (1.in):** Se observă că Treaps și Red-Black Trees au performanțe similare și sunt mai rapide decât Skip Lists în acest scenariu cu un număr mare de inserții.
* **Operații de Ștergere, Căutare, Predecesor, Succesor (2.in - 5.in):** Toate cele trei structuri de date prezintă timpi de execuție relativ mici și comparabili, indicând eficiența lor pentru aceste operații individuale atunci când sunt predominante.
* **Operații de Interval (6.in):** Red-Black Trees par a fi cele mai rapide, urmate de Treaps și apoi Skip Lists pentru interogările de interval.
* **Distribuție Egală (eq.in):** Red-Black Trees au cel mai bun timp general, urmate îndeaproape de Treaps, iar Skip Lists sunt puțin mai lente în acest scenariu echilibrat.

Această analiză preliminară sugerează că, deși toate cele trei structuri oferă performanțe logaritmice bune în general, Red-Black Trees și Treaps tind să aibă un avantaj în scenariile cu multe inserții sau interogări de interval, precum și într-un mix echilibrat de operații. Skip Lists, deși competitive, pot fi puțin mai lente în testele cu un volum mare de inserții. Alegerea optimă a structurii de date va depinde de frecvența specifică și de tipul operațiilor așteptate.