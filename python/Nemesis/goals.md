Main goals (I guess):
- generoi oikeiden siirtojen data treenaukseen move vector formaatissa
- luo treenausfunktiot
    - nemesis.train
        - sekoita muisti
        - generoi move vector neural networkilla
        - laske loss
        - backpropagate
    - nemesis.learn
        - lataa data
        - looppaa learnia
        - tallenna parametrit
    - learn
        - initalisoi model, optimizer ja nemesis
        - learn
- luo treenaustiedosto
    - aseta parametrit
    - learn
- treenaa

Future (sulut):
- treenaa "a" ja "b" parametrit (pelitestaus?)

Side goal(s):
- käytä oikeita arvoja muille muuttujille (kingmoved etc)