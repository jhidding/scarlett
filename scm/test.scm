
(display (operative? $vau) "\n")

($define! $quote
  ($vau (s) #ignore s))

(display (congruent? [pair? ...] [[3 . 4] [4 . 5] [6 . 7] ...]) "\n")
(display (congruent? ($quote ((a b) (c (d e)) ...)) [[1 2] [3 [4 5]] [6 [7 8]] [9 [10 11]]]) "\n")

