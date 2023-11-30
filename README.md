# PACE2024
Repository for the [PACE challenge 2024](https://pacechallenge.org/2024/).


Für das Thema Reduktion zu Directed Feedback Arc Set Problem hab ich mich entschieden, dass die Datenstruktur als ein 2-dimensionaler Vector in C++ dargestellt wird. Mit einem 2D-Vector kann man die Instanzen von PACE 2024 praktisch speichern und auch später für alle Funktionalitäten bereitstellen.

Die Datenstruktur sollte das Format aus PACE 2024 einlesen können und die Daten aus dem Format sollten in den Reihen (repräsentieren die festgelegten Knoten) und in den Spalten (repräsentieren die benachbarten Kanten).

Der Algorithmus braucht noch eine Reduktion zu Directed Feedback Arc Set problem (DFAS), um das originale OCM Problem zu lösen, deswegen kann ich auch die 2D-Vector Datenstruktur für eine Adjacency Liste verbrauchen. Eine Adjacency Liste hilft uns dabei, der gerichtete Graph für das DFAS Problem darzustellen. Wir können diese Datenstruktur dann später für weitere Berechnungen anwenden.

Ich habe außerdem bisher noch kein Gruppenmitglied. Wenn sich jemand auch für das Thema interessiert, könnt ihr hier gern mitteilen oder mir eine Nachricht schreiben.