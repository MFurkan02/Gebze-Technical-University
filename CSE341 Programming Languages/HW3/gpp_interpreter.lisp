
;; Defining the operators of G++.
(defvar OP_PLUS "+")

(defvar OP_MINUS "-")

(defvar OP_DIV "/")

(defvar OP_MULT "*")

(defvar OP_OP "(")

(defvar OP_CP ")")

;; Defining the tokens of the keywords of G++.

(defvar KW_DEF "def")

(defvar KW_EXIT "exit")


 ;; Function to check if the string is a valuef expression.
 ;; It will return 1 if it is.
(defun checkVAL (checkVal)
  (let ((token (string-trim " " checkVal))) ; Adjust the code to trim spaces from the input string
    (let ((b-pos (position #\f token :test #'char=)))
      (if (and b-pos
               (every #'digit-char-p (subseq token 0 b-pos))
               (every #'digit-char-p (subseq token (1+ b-pos))))
          1
          0))))

(defun checkOperator (checkchr)
	(cond
		( (string-equal checkchr "+")1)
		( (string-equal checkchr "-")1)
		( (string-equal checkchr "*")1)
		( (string-equal checkchr "/")1)
		(t 0) ; it will be returned 0 if it is something else
	)
)
; Lisp version of the C code of simplify fnunction in the yacc file
(defun simplify (numerator denominator)
  (if (> numerator denominator)
      (loop for i from denominator downto 1
            until (and (= 0 (mod numerator i))
                       (= 0 (mod denominator i)))
            finally (return i))
      (loop for i from numerator downto 1
            until (and (= 0 (mod numerator i))
                       (= 0 (mod denominator i)))
            finally (return i))))

(defun extract-fraction (fraction-str)
  (let* ((separator-pos (position #\f fraction-str))
         (numerator-str (subseq fraction-str 0 separator-pos))
         (denominator-str (subseq fraction-str (1+ separator-pos))))
    (values (parse-integer numerator-str) (parse-integer denominator-str))))

; Lisp version of the C code of rational operation in the yacc file
(defun perform-operation (operator val1 val2)
  (let ((res-num 0) (res-den 1))
    (multiple-value-bind (num1 den1) (extract-fraction val1)
      (multiple-value-bind (num2 den2) (extract-fraction val2)
        (case operator
          (#\+
           (if (= den1 den2)
               (setf res-num (+ num1 num2) res-den den1)
               (setf res-num (+ (* num1 den2) (* num2 den1))
                      res-den (* den1 den2))))
          (#\-
           (if (= den1 den2)
               (setf res-num (- num1 num2) res-den den1)
               (setf res-num (- (* num1 den2) (* num2 den1))
                      res-den (* den1 den2))))
          (#\*
           (setf res-num (* num1 num2) res-den (* den1 den2)))
          (#\/
           (setf res-num (* num1 den2) res-den (* den1 num2)))
          (t (setf res-num 0 res-den 1)))))
    (let ((gcd (simplify res-num res-den)))
      (setf res-num (/ res-num gcd) res-den (/ res-den gcd))
      (format t "~af~a~%" res-num res-den))))

(defun parse-function (entry)
           (format t "#function
"))

(defun check-identifier (identifier)
  (if (stringp identifier)
      (if (string-match "^[a-zA-Z][a-zA-Z0-9]*$" identifier)
          t
          nil)
      nil))

(defun gpp-interpreter ()
  (format t "Enter g++ to start the interpreter!
")
  (format t "Enter g++ 'filename' to read from file!
")
  (format t "$ ")
  (let* ((gpp-line (read-line)))
    (if (string= "g++" gpp-line)
        (loop
          (format t "> ")
          (let* ((input (read-line)))
            (if (or (string= (subseq input 1 5) KW_EXIT) (string= (subseq input 2 6) KW_EXIT)) ; to exit if the entry is KW_EXIT
                (return)

                (if (and (string= (elt input 0) OP_OP) ; checking if the first and the last character is OP_OP and OP_CP
                         (string= (elt input (- (length input) 1)) OP_CP))
                    ; Exit the loop when (exit) is encountered

                    (if (or (string= (subseq input 1 4) KW_DEF)
                            (string= (subseq input 2 5) KW_DEF))
                        (parse-function input)
                        (let* ((operator (elt input 1))
                               (operands (subseq input 3 (1- (length input))))
                               (space-pos (position #\Space operands))
                               (val1 (subseq operands 0 space-pos))
                               (val2 (subseq operands (1+ space-pos))))
                               (if (= (and (checkoperator operator) (checkval val1) (checkval val2) ) 1)
                          (perform-operation operator val1 val2)
                          (format t "Syntax error!~%"))))
                  (format t "Syntax error!~%")))))





    ; File handling part
    (let* ((filename (subseq gpp-line 4)))
          (with-open-file (file-stream filename :direction :input)
            (loop for line = (read-line file-stream nil)
                  until (null line)
                  do (progn
                       (if (or (string= (subseq line 1 5) KW_EXIT) (string= (subseq line 2 6) KW_EXIT)) ; to exit if the entry is KW_EXIT
                           (return)
                           (if (and (string= (elt line 0) OP_OP) ; checking if the first and the last character is OP_OP and OP_CP
                                    (string= (elt line (- (length line) 1)) OP_CP))
                               ; Exit the loop when (exit) is encountered
                               (if (or (string= (subseq line 1 4) KW_DEF)
                                       (string= (subseq line 2 5) KW_DEF))
                                   (parse-function line)
                                   (let* ((operator (elt line 1))
                                          (operands (subseq line 3 (1- (length line))))
                                          (space-pos (position #\Space operands))
                                          (val1 (subseq operands 0 space-pos))
                                          (val2 (subseq operands (1+ space-pos))))
                                          (if (= (and (checkoperator operator) (checkval val1) (checkval val2) ) 1)
                                     (perform-operation operator val1 val2)
                                     (format t "Syntax error!~%"))))
                               (format t "Syntax error!~%"))))))))))

(gpp-interpreter)