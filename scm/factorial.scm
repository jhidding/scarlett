
($define! fac
  ($lambda (a)
    ($define! loop
      ($lambda (n r)
	($if (zero? n)
	  r
	  (loop (- n 1) (* r n)))))
    (loop a 1)))

(display (fac 10) "\n")

($define! fib
  ($lambda ((a b . c) n)
    ($if (zero? n)
      [a b . c]
      (fib [(+ a b) a b . c] (- n 1)))))

(display (fib [1 1] 10) "\n")

"bye bye!"

