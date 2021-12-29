# CppMuhle

C++ Projekt für die implementation des Brettspiel Mühle auf der Konsole

## Idee der Struktur

Der Controller handelt jegliche Eingaben des Nutzers, interpretiert diese und schickt anweisungen weiter an die Mühle-Logik.
MühleLogik ist das Model und hält den momentanen Stand des spiels und implementiert die Logik des Spiels.
Nach jedem Spielzug wird das aktuelle Spielfeld auf die Konsole gezeichnet in dem die MühleLogik das momentane Spielfelddaten an den KonsolenView weiterschickt.

## Züge

Die Züge werden wie in Schach über eine Notation gelöst. Das Mühle-Feld lässt sich aufteilen in a-g Spalten und 1-7 Zeilen.
In der ersten Phase werden einfach einzelne Koordinaten (z.B. 3c) angegeben.
In der zweiten Phase werden paare an Koordinaten angegeben (z.B. 2b2d).
Dabei muss jeweils geprüft werden ob die eingegebenen Züge legal sind, oder nicht.
