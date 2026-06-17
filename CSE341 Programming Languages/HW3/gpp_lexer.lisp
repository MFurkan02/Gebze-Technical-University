
;; Defining the operators of G++.
(defvar OP_PLUS "+")

(defvar OP_MINUS "-")

(defvar OP_DIV "/")

(defvar OP_MULT "*")

(defvar OP_COMMA ",")

(defvar OP_OP "(")

(defvar OP_CP ")")

( defvar COMMENT ";")


;; Defining the tokens of the keywords of G++.

(defvar KW_AND "and")

(defvar KW_OR "or")

(defvar KW_NOT "not")

(defvar KW_EQUAL "equal")

(defvar KW_LESS "less")

(defvar KW_NIL "nil")

(defvar KW_LIST "list")

(defvar KW_APPEND "append")

(defvar KW_CONCAT "concat")

(defvar KW_SET "set")

(defvar KW_DEF "def")

(defvar KW_FOR "for")

(defvar KW_IF "if")

(defvar KW_EXIT "exit")

(defvar KW_LOAD "load")

(defvar KW_DISPLAY "display")

(defvar KW_TRUE "true")

(defvar KW_FALSE "false")


;; Assigning variables to control if a value,comment,identifier etc. is encountered in the file.

(defvar strToken "")						; All the tokens that will be added to strToken 

(defvar checkValuef 0)						; to check the unsigned fractions it will be 1 when the current char is an unsigned fraction.

(defvar checkOP 0)							; 1 if the current char is an operator

(defvar checkIdentf 0)					    ; 1 if the next char is an identifier

(defvar checkComment 1) 				    ; 1 if the current char is ; (comment token) until the newline

(defvar temp "")							; temporary token

(defvar opTemp "")							; temporary token for operators

(defvar inputFile "input.gpp")				;name of the input file



;; The gppinterpreter function that will check the file char by char 
(defun gppinterpreter (filename)
  (with-open-file (stream filename) 
    (loop :for currChar := (read-char stream nil) :while currChar :collect  
      	(checkToken currChar)      ; reading the file char by char and calling the checkToken function with currentChar
    )
	
  )	
  	strToken ; strToken will be returned
)


 ;; Function to check if the string is a valuef expression.
 ;; It will return 1 if it is.
(defun checkVAL (checkVal)
  (let ((token (string-trim " " checkVal))) ; Adjust the code to trim spaces from the input string
    (let ((b-pos (position #\b token :test #'char=)))
      (if (and b-pos
               (every #'digit-char-p (subseq token 0 b-pos))
               (every #'digit-char-p (subseq token (1+ b-pos))))
          1
          0))))

 ;; Function to check if the string is an operator.
 ;; It will return 1 if it is.
(defun checkOperator (checkchr)
	(cond
		( (string-equal checkchr "(")1)
		( (string-equal checkchr ")")1)
		( (string-equal checkchr "+")1)
		( (string-equal checkchr "-")1)
		( (string-equal checkchr "*")1)
		( (string-equal checkchr "/")1)
		( (string-equal checkchr ",")1)
		(t 0) ; it will be returned 0 if it is something else
	)
)
;; The function to check if the current char is an operator it will directly call the append_to_str function 
;; However if it is an identifier , valuef, or a keyword 
;; it will read it until an operator, space, newline or a tab is encountered 
;; then it will call the append_to_str function 
(defun checkToken (currChar)
	(cond
		;; To check comments
		
		((string-equal currChar COMMENT) ;if the current char is ; lexer shouldn't tokenize until the newline.
			(setf checkComment 1 )
			(setf strToken (concatenate 'string strToken "COMMENT
")))
		((char= currChar #\Newline) ;token is done. add to tokenList. Also, comment is done too.
			(setf checkComment 0)
			(append_to_str temp))
	
		((char= currChar #\Newline) ;token is done. add to strToken. Also, comment is done too.
			(setf checkComment 0)
			(append_to_str temp))
		;; To check esacape char
		((and (eq checkComment 0)(or (char= currChar #\Space ) (char= currChar #\Tab))) ;; After checking adding it to the strToken
			(append_to_str temp))
		;if the current character is an int appends it to temp
	 	((and (eq checkComment 0)(eq (checkVAL currChar) 1))
			(setf temp (concatenate 'string temp (list currChar)))
			(setf checkValuef 1))
		;; to check the situations when there is no in between space (example: 5*5, func(a) a*7)|#
		;if there is a token in temp and current char is an operator 
		((and (eq checkComment 0) (> (length temp) 0)(eq  (checkOperator currChar) 1)) 
			(append_to_str temp)
			(setf opTemp (concatenate 'string opTemp (list currChar)))
			(setf checkOP 1)
			(setf checkIdentf 0)
			(append_to_str opTemp))
		; to check the operators
		((and (eq checkComment 0)(eq (checkOperator currChar) 1))
			(setf opTemp (concatenate 'string opTemp (list currChar)))
			(setf checkOP 1)
			(setf checkIdentf 0)
			(append_to_str opTemp))
		;if the current character is a character we append it to temp string
 		((and (eq checkComment 0)(alpha-char-p currChar))
			(setf temp (concatenate 'string temp (list currChar)))
			(setf checkIdentf 1)
 			(setf checkValuef 0))
		
	)
)
; adds given tokens to strToken according to flags
(defun append_to_str (token) 
	(cond
		;; The conditions to check the errors and show a SYNTAX ERROR message.

		((and (eq checkValuef 1) (char-equal (char temp 0) (char "0" 0))) ;if the first index of a valuef is "0"
			(setf strToken (concatenate 'string strToken "SYNTAX ERROR (FIRST INDEX CAN NOT BE 0!) for " temp " 
"))
			(setf temp "")
			(setf checkValuef 0)) ; After the error we set the check variables to 0 again for the next cond checks.

		((and (eq checkValuef 1) (digit-char-p (char temp 0)) (= checkIdentf 1)) ;if an identifier starts with digit

			(setf strToken (concatenate 'string strToken "SYNTAX ERROR (LEADING DIGIT ERROR!) for " temp "
"))
			(setf temp "")
			(setf checkIdentf 0) ; After the error we set the check variables to 0 again for the next cond checks.
			(setf checkValuef 0)) ; After the error we set the check variables to 0 again for the next cond checks.
		;; OPERATORS
		;; The condition to check operators of G++.
		((eq checkOP 1) 
		  (cond
			((string-equal token OP_PLUS)
	 		(setf strToken (concatenate 'string strToken "OP_PLUS
"))
	 		(setf opTemp "")
	 		(setf checkOP 0)
			)
		
			((string-equal token OP_MINUS)
	 		(setf strToken (concatenate 'string strToken "OP_MINUS
"))
	 		(setf opTemp "")
	 		(setf checkOP 0)
			)

			((string-equal token OP_DIV)
	 		(setf strToken (concatenate 'string strToken "OP_DIV
"))
	 		(setf opTemp "")
	 		(setf checkOP 0)
			)

			((string-equal token OP_MULT)
	 		(setf strToken (concatenate 'string strToken "OP_MULT
"))
	 		(setf opTemp "")
	 		(setf checkOP 0)
			)

			((string-equal token OP_OP)
	 		(setf strToken (concatenate 'string strToken "OP_OP
"))
	 		(setf opTemp "")
	 		(setf checkOP 0)
			)

			((string-equal token OP_CP)
	 		(setf strToken (concatenate 'string strToken "OP_CP
"))
	 		(setf opTemp "")
	 		(setf checkOP 0)
			)

			((string-equal token OP_COMMA)
	 		(setf strToken (concatenate 'string strToken "OP_COMMA
"))
	 		(setf opTemp "")
	 		(setf checkOP 0)
			)
		)
	)


	
		;; The condition to check the keywords of G++.
		; if the token is a string exactly equal to "KW_AND" it will append the appropriate message to the strToken and then will empty the temp variable for the next conds.
		((string-equal token KW_AND)  
			(setf strToken (concatenate 'string strToken "KW_AND
"))
			(setf temp ""))

		; if the token is a string exactly equal to "KW_OR" it will append the appropriate message to the strToken and then will empty the temp variable for the next conds.
	 	((string-equal token KW_OR)
			(setf strToken (concatenate 'string strToken "KW_OR
"))
			(setf temp ""))
	 	;; same conditions as above.
	 	((string-equal token KW_NOT)
			(setf strToken (concatenate 'string strToken "KW_NOT
"))
			(setf temp ""))
	
	 	((string-equal token KW_EQUAL)
			(setf strToken (concatenate 'string strToken "KW_EQUAL
"))
			(setf temp ""))

		((string-equal token KW_LESS)
			(setf strToken (concatenate 'string strToken "KW_LESS
"))
			(setf temp ""))

	 	((string-equal token KW_NIL)
			(setf strToken (concatenate 'string strToken "KW_NILL
"))
			(setf temp ""))

	 	((string-equal token KW_LIST)
			(setf strToken (concatenate 'string strToken "KW_LIST
"))
			(setf temp ""))

		((string-equal token KW_APPEND)
			(setf strToken (concatenate 'string strToken "KW_APPEND
"))
			(setf temp ""))

	 	((string-equal token KW_CONCAT)
			(setf strToken (concatenate 'string strToken "KW_CONCAT
"))
			(setf temp ""))

	 	((string-equal token KW_SET)
			(setf strToken (concatenate 'string strToken "KW_SET
"))
			(setf temp ""))

	 	((string-equal token KW_DEF)
			(setf strToken (concatenate 'string strToken "KW_DEF
"))
			(setf temp ""))

	 	((string-equal token KW_FOR)
			(setf strToken (concatenate 'string strToken "KW_FOR
"))
			(setf temp ""))

	 	((string-equal token KW_IF)
			(setf strToken (concatenate 'string strToken "KW_IF
"))
			(setf temp ""))

		((string-equal token KW_EXIT)
			(setf strToken (concatenate 'string strToken "KW_EXIT
"))
			(setf temp ""))

	 	((string-equal token KW_LOAD)
			(setf strToken (concatenate 'string strToken "KW_LOAD
"))
			(setf temp ""))

	 	((string-equal token KW_DISPLAY)
			(setf strToken (concatenate 'string strToken "KW_DISPLAY
"))
			(setf temp ""))

		((string-equal token KW_TRUE)
			(setf strToken (concatenate 'string strToken "KW_TRUE
"))
			(setf temp ""))

		((string-equal token KW_FALSE)
			(setf strToken (concatenate 'string strToken "KW_FALSE
"))
			(setf temp ""))

		;; VALUEF

	 	((eq checkValuef 1)
	 		(setf strToken (concatenate 'string strToken "VALUEF
"))
	 		(setf temp "")
	 		(setf checkValuef 0))

	 	;; IDENTIFIERs
		
	 	((eq checkIdentf 1)
	 		(setf strToken (concatenate 'string strToken "IDENTIFIER
"))
	 		(setf temp "")
	 		(setf checkIdentf 0)
	 		(setf checkValuef 0))

		
		;( t
		;	(setf strToken (concatenate 'string strToken "SYNTAX ERROR " temp " can not be tokenized!
;"))
;	 		(setf temp "")
;		)
	)
)

(format t "~a" (gppinterpreter inputFile)) ;; calling the lexer with input.gpp file to print the (strToken)
										   ;; that has the syntax tokenizations.