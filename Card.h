#pragma once                      // Sørger for at header-filen kun inkluderes én gang under kompilering

#include <iostream>               // Inkluderer standard input/output (brukes her ved behov)

/// <summary>
/// Klassen Card representerer et kort i memory-spillet.
/// Kortet har en verdi, samt flagg som indikerer om det er matchet eller avslørt.
/// </summary>
class Card {
public:
    // Konstruktør: Oppretter et kort med en gitt verdi.
    // Parameter: value - En heltallsverdi som identifiserer kortet.
    Card(int value);

    // Funksjon for å vise kortet (snur det opp).
    void reveal();

    // Funksjon for å sjekke om kortet allerede er matchet.
    // Returnerer: true hvis kortet er matchet, ellers false.
    bool isMatched() const;

    // Funksjon for å markere kortet som matchet.
    // Etter kall på denne funksjonen skal kortet ikke kunne velges igjen.
    void setMatched();

    // Funksjon for å hente kortets verdi.
    // Returnerer: kortets verdi som en heltall.
    int getValue() const;

    // Funksjon for å sjekke om kortet er synlig (avslørt).
    // Returnerer: true hvis kortet er snudd opp, ellers false.
    bool isRevealed() const;

    // Funksjon for å snu kortet tilbake (skjule kortets verdi).
    void hide();

private:
    int value;      // Variabel som lagrer kortets verdi (f.eks. 0, 1, 2, …)
    bool matched;   // Flag som indikerer om kortet har blitt matchet (true = matchet)
    bool revealed;  // Flag som indikerer om kortet er synlig for spilleren (true = synlig)
};