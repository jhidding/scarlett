
($define! current-environment
  (wrap ($vau () e e)))

($define! $quote
  ($vau (s) #ignore s))

($define! look-up
  (wrap ($vau (s) e (eval s e))))

($define! make-object 
  ($lambda ()
    ($let ((env (current-environment)))
      ($vau (method . args) dyn_env
        ($cond 
	  ((combiner? (look-up method))
	   (eval [method . args] env))

	  (#t (look-up method)))))))

($define! make-struct
  ($vau defs env
    (eval [$define! . defs])
    ($vau (name) #ignore (look-up name))))

($define! some-struct
  ($let ((a 6) (b 43))
    ($vau (name) #ignore
      (eval name))))

(display (some-struct a) " " (some-struct b) "\n")
(display ($quote hello-world!) "\n")

($let ((A (make-struct (a b) [5 42])))
      (display (operative? A) "\n")
      (display (A b) " " (A a) "\n"))

($let ((B (make-object)))
      (B $define! a 8)
      (B $define! b 9)
      (B $define! f ($lambda (x) (* a x)))
      (display (B env) "\n")
      (display (B f (* 7 9)) "\n"))


