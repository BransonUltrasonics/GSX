; $Header: /usr/cvsroot/target/h/wrn/wm/util/layout/ldbcomp/layout.scm,v 1.1.1.1 2001/11/05 17:47:21 tneale Exp $
; Describe the layout of a string of bits

;****************************************************************************
;
;  *** Restricted Rights Legend ***
;
;  The programs and information contained herein are licensed only
;  pursuant to a license agreement that contains use, reverse
;  engineering, disclosure, and other restrictions; accordingly, it
;  is "Unpublished--all rights reserved under the applicable
;  copyright laws".
;
;  Use duplication, or disclosure by the Government is subject to
;  restrictions as set forth in subparagraph (c)(1)(ii) of the Rights
;  in Technical Data and Computer Licensed Programs clause of DFARS
;  52.227 7013.
;
;  Copyright 2000-2001 Wind River Systems, Inc.
;  Copyright 1997 Epilogue Technology Corporation.
;  Copyright 1998 Integrated Systems, Inc.
;  All rights reserved.
;
;  *** Government Use ***
;
;  The Licensed Programs and their documentation were developed at
;  private expense and no part of them is in the public domain.
;
;  The Licensed Programs are "Restricted Computer Software" as that
;  term is defined in Clause 52.227-19 of the Federal Acquisition
;  Regulations (FAR) and are "Commercial Computer Software" as that
;  term is defined in Subpart 227.401 of the Department of Defense
;  Federal Acquisition Regulation Supplement (DFARS).
;
;  (i) If the licensed Programs are supplied to the Department of
;      Defense (DoD), the Licensed Programs are classified as
;      "Commercial Computer Software" and the Government is acquiring
;      only "restricted rights" in the Licensed Programs and their
;      documentation as that term is defined in Clause 52.227
;      7013(c)(1) of the DFARS, and
;
;  (ii) If the Licensed Programs are supplied to any unit or agency
;      of the United States Government other than DoD, the
;      Government's rights in the Licensed Programs and their
;      documentation will be as defined in Clause 52.227-19(c)(2) of
;      the FAR.
;****************************************************************************

(define-integrable (aligned? size align)
  (= 0 (modulo size align)))

;
; The Environment
;

(define layout-working-env (make-symbol-hash-table 100))
(define layout-used-env (make-symbol-hash-table 100))
(define layout-fixed-env (make-symbol-hash-table 100))

(define layout-working-rcs-info (list '()))
(define layout-used-rcs-info (list '()))
(define layout-fixed-rcs-info (list '()))

(define layout-working-C-includes (list '()))
(define layout-used-C-includes (list '()))

(define layout-current-env layout-working-env)
(define layout-current-rcs-info layout-working-rcs-info)
(define layout-current-C-includes layout-working-C-includes)

(define (layout-env-lookup name)
  (hash-table/lookup layout-current-env name
    (lambda (val) val)
    (lambda ()
      (hash-table/lookup layout-used-env name
	(lambda (val) val)
	(lambda ()
	  (hash-table/lookup layout-fixed-env name
	    (lambda (val) val)
	    (lambda ()
	      (err 'layout "Undefined: ~S" name))))))))

(define (layout-env-set! name value)
  (when (or (hash-table/get layout-working-env name #F)
	    (hash-table/get layout-used-env name #F)
	    (hash-table/get layout-fixed-env name #F))
    (format (error-output-port) "Warning: Redefining ~S~%" name))
  (hash-table/put! layout-current-env name value))

(defmac (define-layout name form)
  (%define-layout 'name 'form))

(define (%define-layout name form)
  (let ((val (layout-eval form)))
    (when (and (eq? 'type (layout-val:kind val))
	       (not (layout-type:name (layout-val:data val))))
      (set-layout-type:name (layout-val:data val) name))
    (layout-env-set! name val)
    name))

(define (use-layouts file)
  (unless (string? file)
    (err 'layout "Invalid use-layouts file: ~S" file))
  (let ((saved-env layout-current-env)
	(saved-rcs-info layout-current-rcs-info)
	(saved-C-includes layout-current-C-includes))
    (set! layout-current-env layout-used-env)
    (set! layout-current-rcs-info layout-used-rcs-info)
    (set! layout-current-C-includes layout-used-C-includes)
    (load-layouts file)
    (set! layout-current-env saved-env)
    (set! layout-current-rcs-info saved-rcs-info)
    (set! layout-current-C-includes saved-C-includes)))

(define (show-layout)	; for debugging
  (hash-table/for-each layout-working-env
    (lambda (name value)
      (format #T "~S -> ~S~%" name value)))
  (format #T " -- Used --~%")
  (hash-table/for-each layout-used-env
    (lambda (name value)
      (format #T "~S -> ~S~%" name value)))
  (format #T " -- Fixed --~%")
  (hash-table/for-each layout-fixed-env
    (lambda (name value)
      (format #T "~S -> ~S~%" name value))))

(define (reset-layout)
  (hash-table/clear! layout-working-env)
  (hash-table/clear! layout-used-env)
  (set-car! layout-working-rcs-info '())
  (set-car! layout-used-rcs-info  '())
  (set-car! layout-working-C-includes '())
  (set-car! layout-used-C-includes '()))

(define (fix-layout-env)
  (hash-table/for-each layout-used-env
    (lambda (name value)
      (hash-table/put! layout-fixed-env name value)))
  (hash-table/for-each layout-working-env
    (lambda (name value)
      (hash-table/put! layout-fixed-env name value)))
  (set-car! layout-fixed-rcs-info
	    (append (car layout-working-rcs-info)
		    (car layout-used-rcs-info)
		    (car layout-fixed-rcs-info)))
  (reset-layout))

(defmac (C-include test file)
  (%C-include 'test 'file ))

(define (%C-include test file)
  (set-car! layout-current-C-includes
	    (cons (cons test file)
		  (car layout-current-C-includes))))

(define (define-rcs-info info)
  (unless (and (string? info)
	       (char=? #\$ (string-ref info 0))
	       (let ((end (- (string-length info) 1)))
		 (and (char=? #\$ (string-ref info end))
		      (not (string-search-forwards #\$ info 1 end)))))
    (err 'layout "Invalid RCS keyword string: ~S" info))
  (set-car! layout-current-rcs-info
	    (cons (substring info 1 (- (string-length info) 1))
		  (car layout-current-rcs-info))))

(define (for-each-boilerplate-line proc)
  (for-each proc
	    '("This file was automatically generated by Epilogue Technology's"
	      "network datastructure layout tool."
	      ""
	      "DO NOT MODIFY THIS FILE BY HAND."
	      ""
	      "Source file information:"
	      ))
  (for-each (lambda (x)
	      (proc (string-append " " x)))
	    (reverse (car layout-working-rcs-info)))
  (unless (null? (car layout-used-rcs-info))
    (proc "   --used --")
    (for-each (lambda (x)
		(proc (string-append " " x)))
	      (reverse (car layout-used-rcs-info))))
  (unless (null? (car layout-fixed-rcs-info))
    (proc "   -- fixed --")
    (for-each (lambda (x)
		(proc (string-append " " x)))
	      (reverse (car layout-fixed-rcs-info)))))

;
; Evaluation
;

(define (layout-eval form)
  ((cond ((symbol? form) layout-env-lookup)
	 ((pair? form)
	  (layout-val:combination-handler (layout-eval (car form))))
	 (else make-constant))
   form))

(define (bad-combination form)
  (err 'layout "Bad combination: ~S" form))

(define (check-syntax pattern thing)
  (unless (match? (cdr pattern) (cdr thing))
    (err 'layout "Bad syntax for ~S: ~S" (car pattern) thing)))

(define (check-size form size)
  (unless (and (integer? size)
	       (>= size 0))
    (err 'layout "Bad size ~S: ~S" size form)))

(define-record layout-val
  kind			; constant, type, operator
  data
  combination-handler
  ((disclose self)
   (list "Val"
	 (layout-val:kind self)
	 (layout-val:data self)
	 ))
  )

(define (layout-eval-kind kind form)
  (let ((v (layout-eval form)))
    (unless (eq? kind (layout-val:kind v))
      (err 'layout "~S is not a ~S" form kind))
    (layout-val:data v)))

(define-integrable (layout-eval-constant form)
  (layout-eval-kind 'constant form))

(define (make-constant data)
  (make-layout-val 'constant data bad-combination))

(define-integrable (layout-eval-type form)
  (layout-eval-kind 'type form))

(define (make-type data)
  (make-layout-val 'type data bad-combination))

;
; Types
;

(define-record layout-type
  name			; symbol or #F
  kind			; enumeration, aggregate, array, or some scalar type
  size			; size in bits
  atrs			; alist of attributes, usually '().
  base			; base type for enumeration and array, else #F
  data			; per-kind data:
			;  aggregate: ((name offset type) ...)
			;  enumeration: ((name value) ...) sorted by value
			;  array: (offset (coefficient min max) ...)
			;  else: #F
  ((disclose self)
   (list "Type"
	 (layout-type:name self)
	 (layout-type:kind self)
	 (layout-type:size self)
	 (layout-type:atrs self)
	 (layout-type:base self)
	 ))
  )

;
; Operators in the layout language:
;

(define (def-operator pattern proc)
  (let ((name (car pattern)))
    (hash-table/put! layout-current-env
		     name
		     (make-layout-val 'operator name
				      (lambda (form)
					(check-syntax pattern form)
					(proc form))))
    name))

(defmac (define-layout-macro pattern proc)
  (%define-layout-macro 'pattern proc))

(define (%define-layout-macro pattern proc)
  (def-operator pattern
    (lambda (form)
      (layout-eval (proc form)))))

(def-operator '(sizeof any)
  (lambda (form)
    (make-constant (layout-type:size (layout-eval-type (cadr form))))))

;
; Simple operations on constants:
;

(define (def-constant-operator pattern proc)
  (def-operator pattern
    (lambda (form)
      (make-constant (apply proc (map layout-eval-constant (cdr form)))))))

(def-constant-operator '(+ * any) +)
(def-constant-operator '(- + any) -)
(def-constant-operator '(* * any) *)

(def-constant-operator '(octets * any)
  (lambda args
    (let loop ((l args)
	       (val 0.0))
      (if (null? l)
	  val
	  (loop (cdr l)
		(bitwise-ior (car l) (arithmetic-shift val 8)))))))

;
; Scalar type constructors:
;

(define (def-type-constructor pattern proc)
  (def-operator pattern
    (lambda (form)
      (make-type (apply proc form (map layout-eval-constant (cdr form)))))))

(def-type-constructor '(boolean any)
  (lambda (form size)
    (check-size form size)
    (unless (= size 1)
      (err 'layout "Size ~S must be 1: ~S" size form))
    (make-layout-type #F 'boolean size '() #F #F)))

(def-type-constructor '(opaque any)
  (lambda (form size)
    (check-size form size)
    (make-layout-type #F 'opaque size '() #F #F)))

(def-type-constructor '(signed any)
  (lambda (form size)
    (check-size form size)
    (make-layout-type #F 'signed size '() #F #F)))

(def-type-constructor '(unsigned any)
  (lambda (form size)
    (check-size form size)
    (make-layout-type #F 'unsigned size '() #F #F)))

(def-type-constructor '(signed-8bit-little any)
  (lambda (form size)
    (check-size form size)
    (make-layout-type #F 'signed-8bit-little size '() #F #F)))

(def-type-constructor '(unsigned-8bit-little any)
  (lambda (form size)
    (check-size form size)
    (make-layout-type #F 'unsigned-8bit-little size '() #F #F)))

;
; Helpers for things that have clauses (struct, union, array, enum):
;

;NOTES.  The general theory for things that have clauses is to -ignore-
; clauses that aren't defined.  This lets you mark up layouts with
; information that isn't used now but might be used in the future.  Of
; course you give up some error checking by doing this.  Perhaps that makes
; it a bad idea?

(define clause-patterns
  '((struct * (symbol * any))
    (union * (symbol * any))
    (field symbol any)
    (value symbol any)
    (fill ? any)
    (type any)
    (size any)
    (align any)
    (count any)
    (spacing any)
    (base (or 8 10 16))))

(define (check-clause-syntax clause)
  ;; Assuming `clause' is a cons whose car is a symbol.
  (let ((pattern (assq (car clause) clause-patterns)))
    (when pattern
      (check-syntax pattern clause))))

(define (clause-value form key kind default? default)
  (let loop ((clauses (cdr form)))
    (cond ((null? clauses)
	   (unless default?
	     (err 'layout "Missing ~S clause: ~S" key form))
	   default)
	  ((eq? key (caar clauses))
	   (when (assq key (cdr clauses))
	     (err 'layout "More than one ~S clause: ~S" key form))
	   (if (eq? kind 'literal)
	       (cadar clauses)
	       (layout-eval-kind kind (cadar clauses))))
	  (else
	   (loop (cdr clauses))))))

;
; Enumerations
;

(define enumeration-processors (make-symbol-hash-table 25))

(def-operator '(enum * (symbol * any))
  (lambda (form)
    (let* ((type (clause-value form 'type 'type #F #F))
	   (process (or (hash-table/get enumeration-processors
					(layout-type:kind type)
					#F)
			(err 'layout
			     "Don't know how to enumerate in this type: ~S"
			     form))))
      (let loop ((clauses (cdr form))
		 (out '()))
	(if (null? clauses)
	    (make-type (make-layout-type #F
					 'enumeration
					 (layout-type:size type)
					 (list (cons 'base
						     (clause-value form
								   'base
								   'literal
								   #T
								   10)))
					 type
					 ;; preserve original clause order for
					 ;; duplicate values:
					 (sort-list! (reverse! out)
					   (lambda (x y)
					     (< (cadr x) (cadr y))))))
	    (let ((clause (car clauses)))
	      (check-clause-syntax clause)
	      (cond ((eq? (car clause) 'value)
		     (when (assq (cadr clause) out)
		       (err 'layout "Name ~S used more than once: ~S"
			    (cadr clause) form))
		     (loop (cdr clauses)
			   (cons (list (cadr clause)
				       (process form
						type
						(cadr clause)
						(layout-eval-constant
						  (caddr clause))))
				 out)))
		    (else 
		     (loop (cdr clauses) out)))))))))

(define (process-unsigned-enumeration form type name val)
  (let ((size (layout-type:size type)))
    (unless (and (integer? val)
		 (<= 0 val)
		 (< val (expt 2 size)))
      (err 'layout "Bad value ~S for ~S in: ~S" val name form))
    val))

(define (process-signed-enumeration form type name val)
  (let ((size (layout-type:size type)))
    (unless (and (integer? val)
		 (<= (- (expt 2 (- size 1))) val)
		 (< val (expt 2 (- size 1))))
      (err 'layout "Bad value ~S for ~S in: ~S" val name form))
    val))

(hash-table/put! enumeration-processors 'unsigned-8bit-little
		 process-unsigned-enumeration)

(hash-table/put! enumeration-processors 'unsigned
		 process-unsigned-enumeration)

(hash-table/put! enumeration-processors 'opaque
		 process-unsigned-enumeration)

(hash-table/put! enumeration-processors 'signed-8bit-little
		 process-signed-enumeration)

(hash-table/put! enumeration-processors 'signed
		 process-signed-enumeration)

;
; Aggregates
;

(define (process-aggregate form)
  (let ((group (get-aggregate-group 8 form)))
    (let loop ((specs (cdr group))
	       (out '()))
      (if (null? specs)
	  (make-type (make-layout-type #F 'aggregate (car group) '() #F out))
	  (let ((spec (car specs)))
	    (when (assq (cadr spec) out)
	      (err 'layout "Field name ~S used more than once: ~S"
		   (cadr (car specs)) form))
	    (loop (cdr specs)
		  (cons (list (cadr spec) (car spec) (caddr spec))
			out)))))))

(def-operator '(struct * (symbol * any)) process-aggregate)
(def-operator '(union * (symbol * any)) process-aggregate)

(define (get-aggregate-group align form)
  ;; returns a "group", which is of the form: (size (offset name type) ...)
  (for-each check-clause-syntax (cdr form))
  (let ((align (clause-value form 'align 'constant #T align)))
    (define (process-clause clause)
      (let* ((group (case (car clause)
		      ((struct union)
		       (get-aggregate-group align clause))
		      ((fill)
		       (list (if (null? (cdr clause))
				 #F
				 (layout-eval-constant (cadr clause)))))
		      ((field)
		       (let ((type (layout-eval-type (caddr clause))))
			 (list (layout-type:size type)
			       (list 0 (cadr clause) type))))
		      (else
		       (list 0))))
	     (size (car group)))
	(unless (or (not size)
		    (aligned? size align))
	  (err 'layout "size ~S is not a multiple of ~S: ~S"
	       size align clause))
	group))
    (let* ((union? (eq? 'union (car form)))
	   (groups (map process-clause (cdr form)))
	   (minsize (reduce-map (if union? max +)
				0
				(lambda (group) (or (car group) 0))
				groups))
	   (size (clause-value form 'size 'constant #T minsize)))
      (unless (aligned? size align)
	(err 'layout "requested size ~S is not a multiple of ~S: ~S"
	     size align form))
      (unless (>= size minsize)
	(err 'layout "size ~S is smaller than the minimum of ~S: ~S"
	     size minsize form))
      (unless union?
	(let ((p (assq #F groups)))
	  (cond (p (set-car! p (- size minsize)))
		((not (= size minsize))
		 (err 'layout "size ~S is incorrect, computed size is ~S: ~S"
		      size minsize form)))))
      (unless (not (assq #F groups))
	(err 'layout "too many unspecified fill clauses: ~S" form))
      (let loop ((groups groups)
		 (offset 0)
		 (specs '()))
	(if (null? groups)
	    (cons size specs)
	    (loop (cdr groups)
		  (if union?
		      0
		      (+ offset (caar groups)))
		  (map* specs
			(lambda (spec)
			  (cons (+ offset (car spec))
				(cdr spec)))
			(cdar groups))))))))

;
; Arrays
;

(def-operator '(array * (symbol * any))
  (lambda (form)
    (for-each check-clause-syntax (cdr form))
    (let* ((align (clause-value form 'align 'constant #T 8))
	   (type (clause-value form 'type 'type #F #F))
	   (typesize (layout-type:size type))
	   (spacing (clause-value form 'spacing 'constant #T typesize))
	   (count (clause-value form 'count 'constant #F #F))
	   (default-size (* spacing count))
	   (minsize (- default-size (- spacing typesize)))
	   (size (clause-value form 'size 'constant #T default-size)))
      (unless (>= spacing typesize)
	(err 'layout "spacing ~S is smaller than the minimum of ~S: ~S"
	     spacing typesize form))
      (unless (aligned? spacing align)
	(err 'layout "spacing of ~S is not a multiple of ~S: ~S"
	     spacing align form))
      (check-size form size)
      (unless (>= size minsize)
	(err 'layout "size ~S is smaller than the minimum of ~S: ~S"
	     size minsize form))
      (unless (aligned? size align)
	(err 'layout "requested size ~S is not a multiple of ~S: ~S"
	     size align form))
      (make-type (make-layout-type #F 'array size '() type
				   (list 0 (list spacing 0 (- count 1))))))))

;
; Output driver loops
;

; The new way:
(define (generate-layout-defs gen-def)
  (hash-table/for-each layout-working-env
    (lambda (name val)
      (when (eq? 'type (layout-val:kind val))
	(gen-def name (generate-layout-defs-type (layout-val:data val)))))))

(define (generate-layout-defs-subtype type)
  (or (layout-type:name type)
      (generate-layout-defs-type type)))

(define (generate-layout-defs-type type)
  ;; <type> :=
  ;;   <name>
  ;;   (<kind> <size> <atrs>)
  ;;   (aggregate <size> <atrs> (<name> <offset> <type>) ...)
  ;;   (enumeration <size> <atrs> <type> (<name> <value>))
  ;;   (array <size> <atrs> <type> <offset> (<coefficient> <min> <max>) ...)
  ;; Note that the array coefficients are in -reverse- order!
  (let ((kind (layout-type:kind type))
	(base (layout-type:base type))
	(data (layout-type:data type)))
    (cons* kind
	   (layout-type:size type)
	   (layout-type:atrs type)
	   (case kind
	     ((aggregate)
	      (map (lambda (triple)
		     (list (car triple)
			   (cadr triple)
			   (generate-layout-defs-subtype (caddr triple))))
		   data))
	     ((enumeration array)
	      (cons (generate-layout-defs-subtype base)
		    data))
	     (else '())))))

; The old way:
(define (generate-layout gen-header gen-size gen-enum-defs gen-ptr gen-refset)
  (hash-table/for-each layout-working-env
    (lambda (name val)
      (when (eq? 'type (layout-val:kind val))
	(let ((type (layout-val:data val)))
	  (when (eq? name (layout-type:name type))
	    (gen-header name)
	    (let ((total-size (layout-type:size type)))
	      (define (recur path offset dope type)
		(let ((recur? (or (null? path)
				  (not (layout-type:name type))))
		      (kind (layout-type:kind type))
		      (size (layout-type:size type))
		      (atrs (layout-type:atrs type))
		      (base (layout-type:base type))
		      (data (layout-type:data type)))
		  (case kind
		    ((aggregate)
		     (when recur?
		       (gen-size name path size atrs)
		       (for-each (lambda (triple)
				   (let ((path (cons (car triple) path))
					 (offset (+ offset (cadr triple)))
					 (type (caddr triple)))
				     (gen-ptr name path offset dope
					      (layout-type:size type)
					      (layout-type:atrs type))
				     (recur path offset dope type)))
				 data)))
		    ((array)
		     (when recur?

		       ;; Each element in `path' is either a field name
		       ;; symbol or the string "ELT" for an array
		       ;; reference.  Each element in `dope' is a
		       ;; (coefficient min max) triple.  Both lists are in
		       ;; reverse order.  If you don't want to handle
		       ;; arrays, don't.

		       (gen-size name path size atrs)
		       (let ((path (cons "ELT" path))
			     (offset (+ offset (car data)))
			     (dope (append (cdr data) dope)))
			 (gen-ptr name path offset dope
				  (layout-type:size base)
				  (layout-type:atrs base))
			 (recur path offset dope base))))
		    ((enumeration)
		     (when (null? path)
		       (gen-size name path size atrs))
		     (let ((kind (layout-type:kind base)))
		       (gen-refset name path offset dope kind
				   size atrs total-size)
		       (when recur?
			 (gen-enum-defs name path kind size atrs data))))
		    (else
		     (when (null? path)
		       (gen-size name path size atrs))
		     (gen-refset name path offset dope kind
				 size atrs total-size)))))
	      (recur '() 0 '() type))))))))

;
; Output Scheme->C header file:
;

; The new way:
(define (generate-s2c-layout-definitions port schfile)
  (for-each-boilerplate-line
    (lambda (line)
      (format port "; ~A~%" line)))
  (newline port)
  (generate-layout-defs (lambda (name type)
			  (write `(define-layout-macro ,name ,type) port)
			  (newline port))))

; The old way:
(define (generate-old-s2c-layout-definitions port)

  (define (out expr)
    (write expr port)
    (newline port))

  (define (cant sym ctl . args)
    (format port "; Can't define ~S because:~%; " sym)
    (apply format port ctl args)
    (newline port))

  (define (mksym name path tail)
    (let loop ((path path)
	       (tail tail))
      (cond ((null? path)
	     (apply symbol-append name tail))
	    (else
	     (loop (cdr path)
		   (cons '/ (cons (car path) tail)))))))

  (define (mkixs l)
    (let loop ((in l)
	       (out '())
	       (n 0))
      (if (null? in)
	  out
	  (loop (cdr in)
		(cons (symbol-append 'x n) out)
		(+ 1 n)))))

  (define (mksum p const coeffs vars)
    (let loop ((coeffs coeffs)
	       (vars vars)
	       (terms '()))
      (cond ((not (null? coeffs))
	     (loop (cdr coeffs)
		   (cdr vars)
		   (cond ((= 0 (car coeffs))
			  terms)
			 ((= 1 (car coeffs))
			  (cons (car vars) terms))
			 ((= -1 (car coeffs))
			  (cons `(- ,(car vars)) terms))
			 (else
			  (cons `(* ,(car coeffs) ,(car vars)) terms)))))
	    ((not (= 0 const))
	     `(+ ,p ,const ,@terms))
	    ((not (null? terms)) `(+ ,p ,@terms))
	    (else p))))

  (define (gen-header name)
    (format port "~%; Definitions for ~S~%~%" name))

  (define (gen-size name path size atrs)
    (let ((sym (mksym name path '(/size))))
      (if (aligned? size 8)
	  (out `(define-constant ,sym ,(/ size 8)))
	  (cant sym "Size ~S is not a multiple of 8." size))))

  (define (gen-enum-defs name path kind size atrs pairs)
    ;; In theory the `kind' and `size' can be used here to adjust the
    ;; value of the constant.  For example, if `kind' is `opaque'
    ;; and `size' is 16, we might arrange here to byteswap the value
    ;; (assuming that the `-ref' and `-set!' routines join in the
    ;; conspiracy).  In the Scheme->C version, we treat `opaque' just like
    ;; `unsigned'.
    (out `(define ,(mksym name path '(/values)) ',pairs))
    (for-each (lambda (pair)
		(out `(define-constant ,(mksym name path (list '= (car pair)))
			,(cadr pair))))
	      pairs))

  (define (gen-ptr name path offset dope size atrs)
    (let ((sym (mksym name path '(/ptr)))
	  (ixs (mkixs dope)))
      (cond ((not (aligned? size 8))
	     (cant sym "Size ~S is not a multiple of 8." size))
	    ((not (aligned? offset 8))
	     (cant sym "Offset ~S is not a multiple of 8." offset))
	    ((there-exists? dope
	       (lambda (elt)
		 (not (aligned? (car elt) 8))))
	     (cant sym
		   "Array elements are not aligned on octet boundaries: ~S"
		   dope))
	    (else
	     ;; No bounds checking on arrays...
	     (out `(define-in-line (,sym p ,@(reverse ixs))
		     ,(mksum 'p
			     (/ offset 8)
			     (map (lambda (elt) (/ (car elt) 8)) dope)
			     ixs)))))))

  (define (gen-refset name path offset dope kind size atrs total-size)
    (let ((refsym (mksym name path '(/ref)))
	  (setsym (mksym name path '(/set!)))
	  (ixs (mkixs dope))
	  (bytes? (and (aligned? size 8)
		       (aligned? offset 8)
		       (for-all? dope (lambda (elt) (aligned? (car elt) 8)))))
	  (signed? (memq kind '(signed signed-8bit-little)))
	  (littlendian? (memq kind
			      '(signed-8bit-little unsigned-8bit-little))))
      (cond ((not (memq kind '(opaque
			       boolean signed unsigned
			       signed-8bit-little unsigned-8bit-little)))
	     (cant (list refsym setsym) "Unsupported scalar type: ~S" kind))
	    ((and littlendian? (not bytes?))
	     (cant (list refsym setsym)
		   "Unaligned little endian: offset ~S size ~S dope ~S"
		   offset size dope))
	    (bytes?
	     (let ((size (/ size 8))
		   (pexpr (mksum 'p
				 (/ offset 8)
				 (map (lambda (elt) (/ (car elt) 8)) dope)
				 ixs)))
	       (out `(define-in-line (,refsym b p ,@(reverse ixs))
		       (,(if signed?
			     (if littlendian?
				 `signed-littlendian-bytefield-ref
				 `signed-bigendian-bytefield-ref)
			     (if littlendian?
				 `unsigned-littlendian-bytefield-ref
				 `unsigned-bigendian-bytefield-ref))
			b ,pexpr ,size)))
	       (out `(define-in-line (,setsym b p ,@(reverse ixs) val)
		       (,(if littlendian?
			     `littlendian-bytefield-set!
			     `bigendian-bytefield-set!)
			b ,pexpr ,size val)))))
	    (else
	     (let ((pexpr (mksum '(* 8 p) offset (map car dope) ixs)))
	       (out `(define-in-line (,refsym b p ,@(reverse ixs))
		       (,(if signed?
			     (if littlendian?
				 `signed-littlendian-bitfield-ref
				 `signed-bigendian-bitfield-ref)
			     (if littlendian?
				 `unsigned-littlendian-bitfield-ref
				 `unsigned-bigendian-bitfield-ref))
			b ,pexpr ,size)))
	       (out `(define-in-line (,setsym b p ,@(reverse ixs) val)
		       (,(if littlendian?
			     `littlendian-bitfield-set!
			     `bigendian-bitfield-set!)
			b ,pexpr ,size val))))))))

  
  (for-each-boilerplate-line
    (lambda (line)
      (format port "; ~A~%" line)))
  (newline port)
  (generate-layout gen-header gen-size gen-enum-defs gen-ptr gen-refset)

  )

;
; Check the uniqueness of generated names:
;

(define name-length-limit 31)
(define name-length-table (make-string-hash-table 100))

(define (check-name-length name)
  (hash-table/modify!
      name-length-table
      (if (> (string-length name) name-length-limit)
	  (substring name 0 name-length-limit)
	  name)
      '()
    (lambda (l) (cons name l)))
  name)

(define (first-difference s1 s2)
  (let ((l1 (string-length s1))
	(l2 (string-length s2)))
    (let ((stop (min l1 l2)))
      (let loop ((i 0))
	(if (< i stop)
	    (if (char=? (string-ref s1 i)
			(string-ref s2 i))
		(loop (+ 1 i))
		i)
	    (if (= l1 l2)
		#F
		i))))))

(define (write-n-chars n c port)
  (let loop ((n n))
    (when (> n 0)
      (write-char c port)
      (loop (- n 1)))))

(define (report-name-length-problems hfile)
  (hash-table/for-each name-length-table
    (lambda (prefix names)
      (cond ((not (null? (cdr names)))
	     (format (error-output-port)
	       "Warning: In ~A, the first ~A characters are the same in:~%"
	       hfile name-length-limit)
	     (let loop ((names (sort-list! names string<?))
			(min-i #F)
			(max-i #F))
	       (display (car names) (error-output-port))
	       (newline (error-output-port))
	       (if (null? (cdr names))
		   (when min-i
		     (write-n-chars name-length-limit
				    #\-
				    (error-output-port))
		     (write-n-chars (- min-i name-length-limit)
				    #\=
				    (error-output-port))
		     (write-n-chars (+ 1 (- max-i min-i))
				    #\#
				    (error-output-port))
		     (format (error-output-port) " (must shorten by ~A)~%"
		       (+ 1 (- max-i name-length-limit))))
		   (let ((i (first-difference (car names) (cadr names))))
		     (loop (cdr names)
			   (if i (if min-i (min i min-i) i) min-i)
			   (if i (if max-i (max i max-i) i) max-i)))))))))
  (hash-table/clear! name-length-table))

;
; Output C header file:
;

(define (generate-C-layout-definitions port hfile include-check)

  ;; This whole thing was written under the assumption that we only support
  ;; the unsigned types.  If you're thinking about adding support for the
  ;; signed scalar types BE CAREFUL!

  (define (out ctl . args)
    (apply format port ctl args))

  (define (outdefmac nam ixs)
    (out "#define ~A(_P_" nam)
    (for-each (lambda (ix)
		(out ", ~A" ix))
	      (reverse ixs)))

  (define (mks ctl . args)
    (apply format #F ctl args))

  (define (cant name ctl . args)
    (format port "/* Can't define ~A because:~%   " name)
    (apply format port ctl args)
    (display "   */" port)
    (newline port))

  (define (include-check-name file)
    (list->string (map (lambda (c)
			 (cond ((char-alphabetic? c)
				(char-upcase c))
			       ((char-numeric? c)
				c)
			       (else #\_)))
		       (string->list (pathname-entry-name file)))))

  (define (namify x tail)
    (cond ((symbol? x)
	   ;; -!- Should error check...
	   (cons* "_" (symbol->string x) tail))
	  ((string? x) (cons* "_" x tail))
	  ((null? x) tail)
	  ((pair? x)
	   ;; Lists are reversed to make it easy to deal with a `path':
	   (namify (cdr x) (namify (car x) tail)))
	  (else (err 'layout "Illegal name: ~S" x))))

  (define (mknam . args)
    (check-name-length (apply string-append (cdr (fold namify args)))))

  (define (mkixs l)
    (let loop ((in l)
	       (out '())
	       (n 0))
      (if (null? in)
	  out
	  (loop (cdr in)
		(cons (mks "_X~S_" n) out)
		(+ 1 n)))))

  (define (mksum p const coeffs vars)
    (let loop ((coeffs coeffs)
	       (vars vars)
	       (terms ""))
      (cond ((not (null? coeffs))
	     (loop (cdr coeffs)
		   (cdr vars)
		   (cond ((= 0 (car coeffs))
			  terms)
			 ((= 1 (car coeffs))
			  (mks "+(~A)~A" (car vars) terms))
			 ((= -1 (car coeffs))
			  (mks "-(~A)~A" (car vars) terms))
			 ((> 0 (car coeffs))
			  (mks "-(~A)*(~A)~A"
			       (mknum (- (car coeffs)))
			       (car vars)
			       terms))
			 (else
			  (mks "+(~A)*(~A)~A"
			       (mknum (car coeffs))
			       (car vars)
			       terms)))))
	    ((= 0 const)
	     (mks "(~A)~A" p terms))
	    ((> 0 const)
	     (mks "(~A)-(~A)~A" p (mknum (- const)) terms))
	    (else
	     (mks "(~A)+(~A)~A" p (mknum const) terms)))))

  (define (mknum n)
    (unless (integer? n)
      (err 'layout "Not an integer: ~S" n))
    (encode-integer-in-base n 10 "0123456789"))

  (define (mksize size)
    (cond ((<= size 8) "08")
	  ((<= size 16) "16")
	  ((<= size 32) "32")
	  (else (err 'layout "Size more than 32: ~S" size))))

  (define (mkcast size x)
    ;; The GLUE_CASTnn() family should cast the value to the same type that
    ;; the GLUE_Gxnn() family returns.
    (mks "GLUE_CAST~A(~A)" (mksize size) x))

  (define (mkopaque size n base)
    ;; The GLUE_OPAQUEnn() family should return the same type and values as
    ;; GLUE_GUnn() returns.
    (mks "GLUE_OPAQUE~A(~A)"
	 (mksize size)
	 (mkconst size n base)))

  (define (mkconst size n base)
    ;; Caller's responsibility to cope with signed/unsigned issues
    (mkcast size
	    (if (zero? n)
		"0L"
		(mks (case base
		       ((8) "0~AL")
		       ((10) "~AL")
		       ((16) "0x~AL")
		       (else (err 'layout "Bad base: ~S" base)))
		     (encode-integer-in-base n base "0123456789ABCDEF")))))

  ;; The next four deal with extracting a field from a loaded value

  (define (mkldb shift size osize get)
    (let ((mask (mkconst osize
			 (arithmetic-shift (- (arithmetic-shift 1 size) 1)
					   shift)
			 16)))
      (cond ((= osize size) get)
	    ((= 0 shift)
	     (mkcast size (mks "~A & ~A"
			       get mask)))
	    ((= osize (+ size shift))
	     (mkcast size (mks "~A >> ~A"
			       get (mknum shift))))
	    (else
	     (mkcast size (mks "(~A & ~A) >> ~A"
			       get mask (mknum shift)))))))

  (define (mkdpb shift size osize get val)
    (let ((mask (mkconst osize
			 (arithmetic-shift (- (arithmetic-shift 1 size) 1)
					   shift)
			 16))
	  (val (mkcast osize val)))
      (cond ((= osize size) val)
	    ((= 0 shift)
	     (mks "(~A & ~~~A) | (~A & ~A)"
		  get mask val mask))
	    (else
	     (mks "(~A & ~~~A) | ((~A << ~A) & ~A)"
		  get mask val (mknum shift) mask)))))

  (define (mkldbool shift osize get)
    (let ((mask (mkconst osize (arithmetic-shift 1 shift) 16)))
      (mks "(~A & ~A)" get mask)))

  (define (mkdpbool shift osize get val)
    (let ((mask (mkconst osize (arithmetic-shift 1 shift) 16)))
      (mks "((~A) ? ~A | ~A : ~A & ~~~A)" val get mask get mask)))

  ;; If you mess with the next three, you better understand all the issues.
  ;; These guys decide what bytes to load to obtain a given field.

  (define (byte-offset offset size tsize)
    ;; Compute the byte offset most likely to result in an aligned
    ;; reference to a quantity at the given offset and size.
    ;; (While staying within the object (if possible).)
    (let ((ldsize (+ size (modulo offset 8))))
      (cond ((<= ldsize 16) (quotient offset 8))	;;; 1 or 2 bytes
	    ((<= ldsize 24)				;;; 3 bytes
	     (let ((boffset (* 2 (quotient offset 16))))
	       ;; If the proposed 32-bit load would take us beyond the end of
	       ;; the object, and there is room to back up, then back up.
	       (if (and (> (+ 32 (* 8 boffset))
			   tsize)
			(>= offset 8))
		   (- (quotient offset 8) 1)
		   boffset)))
	    ((<= ldsize 32) (quotient offset 8))	;;; 4 bytes
	    (else #F))))				;;; too many

  (define (outer-size offset size)
    ;; Compute the size field to load
    (let ((ldsize (+ size (modulo offset 8))))
      (cond ((<= ldsize  8)  8)				;;; 1 byte
	    ((<= ldsize 16) 16)				;;; 2 bytes
	    ((<= ldsize 32) 32)				;;; 3 or 4 bytes
	    (else #F))))				;;; too many

  (define (bit-shift boffset osize offset size)
    ;; Compute the bit offset within the loaded field
    ;; Note that this is a little-endian quantity!
    (- (+ osize (* 8 boffset))
       (+ size offset)))

  ;; Finally, the routines that are actually called by the driver:

  (define (gen-header name)
    (out "~%/* Definitions for ~S */~%~%" name))

  (define (gen-size name path size atrs)
    (let ((nam (mknam "SIZEOF" name path)))
      (if (aligned? size 8)
	  (out "#define ~A (~A)~%" nam (mknum (/ size 8)))
	  (cant nam "Size ~S is not a multiple of 8." size))))

  (define (gen-enum-defs name path kind size atrs pairs)
    ;; `kind' and `size' should be used here to adjust the
    ;; value of the constant.  For example, if `kind' is `opaque'
    ;; and `size' is 16, we might arrange here to byteswap the value
    ;; (assuming that the `_REF' and `_SET' routines join in the
    ;; conspiracy).
    (let ((base (cdr (assq 'base atrs))))
      (for-each (case kind
		  ((opaque)
		   (lambda (pair)
		     (out "#define ~A ~A~%"
			  (mknam name path "is" (car pair))
			  (mkopaque size (cadr pair) base))))
		  ((unsigned unsigned-8bit-little)
		   (lambda (pair)
		     (out "#define ~A ~A~%"
			  (mknam name path "is" (car pair))
			  (mkconst size (cadr pair) base))))
		  (else (cant (mknam name path "is" "*")
			      "Unsupported scalar type: ~S" kind)))
		pairs)))

  (define (gen-ptr name path offset dope size atrs)
    (let ((nam (mknam "PTR" name path))
	  (ixs (mkixs dope)))
      (cond ((not (aligned? size 8))
	     (cant nam "Size ~S is not a multiple of 8." size))
	    ((not (aligned? offset 8))
	     (cant nam "Offset ~S is not a multiple of 8." offset))
	    ((there-exists? dope
	       (lambda (elt)
		 (not (aligned? (car elt) 8))))
	     (cant nam
		   "Array elements are not aligned on octet boundaries: ~S"
		   dope))
	    (else
	     ;; No bounds checking on arrays...
	     (outdefmac nam ixs)
	     (out ")\\~% (~A)~%"
		  (mksum "GLUE_CAST_PTR(_P_)"
			 (/ offset 8)
			 (map (lambda (elt) (/ (car elt) 8)) dope)
			 ixs))))))

  (define (gen-refset name path offset dope kind size atrs total-size)
    (let ((getnam (mknam "GET" name path))
	  (setnam (mknam "SET" name path))
	  (ixs (mkixs dope))
	  (boffset (byte-offset offset size total-size))
	  (osize (outer-size offset size)))
      (cond ((not (memq kind '(opaque boolean unsigned unsigned-8bit-little)))
	     (cant (list getnam setnam) "Unsupported scalar type: ~S" kind))
	    ((> size 32)
	     (cant (list getnam setnam)
		   "Size ~S is greater than 32." size))
	    ((not (and boffset osize))
	     (cant (list getnam setnam)
		   "Requires load of more than 32 bits: offset ~S size ~S"
		   offset size))
	    ((there-exists? dope
	       (lambda (elt)
		 (not (aligned? (car elt) 8))))
	     (cant (list getnam setnam)
		   "Array elements are not aligned on octet boundaries: ~S"
		   dope))
	    ;; For the cases that involve byte swapping in some form, don't
	    ;; get involved in shifting and masking later:
	    ((and (memq kind '(opaque unsigned-8bit-little))
		  (not (and (aligned? offset 8)
			    (or (= size 8)
				(= size 16)
				(= size 32)))))
	     (cant (list getnam setnam)
		   "Unaligned ~S: offset ~S size ~S dope ~S"
		   kind offset size dope))
	    (else
	     (let* ((shift (bit-shift boffset osize offset size))
		    (kind-char (case kind
				 ((opaque) "U")
				 ((unsigned-8bit-little) "L")
				 (else "B")))	; unsigned or boolean
		    (size-chars (mksize osize))
		    (hint24 (cond ((not (= osize 32)) "")
				  ((<= 8 shift) "MS24")
				  ((>= 24 (+ shift size)) "LS24")
				  (else "")))
		    (pexpr (mksum "GLUE_CAST_PTR(_P_)"
				  boffset
				  (map (lambda (elt) (/ (car elt) 8)) dope)
				  ixs))
		    (getexpr (mks "GLUE_G~A~A~A(~A)"
				  kind-char size-chars hint24 pexpr)))
	       (outdefmac getnam ixs)
	       (out ")\\~% ~A~%"
		    (if (eq? kind 'boolean)
			(mkldbool shift osize getexpr)
			(mkldb shift size osize getexpr)))
	       (outdefmac setnam ixs)
	       (out ", _V_)\\~% GLUE_S~A~A~A(~A, ~A)~%"
		    kind-char size-chars hint24 pexpr
		    (if (eq? kind 'boolean)
			(mkdpbool shift osize getexpr "_V_")
			(mkdpb shift size osize getexpr "_V_")))
	       )))))

  (let ((h-name (include-check-name hfile))
	(first? #T))
    (for-each-boilerplate-line
      (lambda (line)
	(cond (first?
	       (out "/* ~A~%" line)
	       (set! first? #F))
	      (else (out " * ~A~%" line)))))
    (unless first?
      (out " */~%"))
    (when include-check
      (out "~%#ifndef ~A~A~%#define ~A~A~%"
	   include-check h-name
	   include-check h-name))
    (unless (null? (car layout-working-C-includes))
      (newline port))
    (for-each (lambda (pair)
		(if (car pair)
		    (out "#ifndef ~S~%#include ~S~%#endif~%"
			 (car pair)
			 (cdr pair))
		    (out "#include ~S~%" (cdr pair))))
	      (reverse (car layout-working-C-includes)))
    (generate-layout gen-header gen-size gen-enum-defs gen-ptr gen-refset)
    (when include-check
      (out "~%#endif /* ~A~A */~%"
	   include-check h-name)))

  (report-name-length-problems hfile)
  )

;
; Command line interface
;

(define user-layouts-package #F)

(define (load-layouts name)
  (unless user-layouts-package
    (set! user-layouts-package (get-package 'user-layouts)))
  (with-interaction-environment user-layouts-package
    (lambda ()
      (load name))))

(define (do-layout-command-line args)
  (let ((alist (parse-options (cdr args)
			      '(("-define" arg defs)
				("-makesch" arg schfiles)
				("-makeoldsch" arg oldschfiles)
				("-makeh" arg hfiles)
				("-include-check" arg include-check)
				("-unique-names" arg unique-names)))))
    (let ((unique-names (assq 'unique-names alist)))
      (unless (null? (cdr unique-names))
	(set! name-length-limit
	      (string->number (car (last-pair (cdr unique-names)))))))
    (for-each load-layouts (cdr (assq 'defs alist)))
    (fix-layout-env)
    (for-each load-layouts (cdr (assq 'args alist)))
    (for-each (lambda (oldschfile)
		(call-with-output-file oldschfile
		  generate-old-s2c-layout-definitions))
	      (cdr (assq 'oldschfiles alist)))
    (for-each (lambda (schfile)
		(call-with-output-file schfile
		  (lambda (port)
		    (generate-s2c-layout-definitions port schfile))))
	      (cdr (assq 'schfiles alist)))
    (let ((include-check (let ((ic (assq 'include-check alist)))
			   (if (null? (cdr ic))
			       #F
			       (car (last-pair (cdr ic)))))))
      (for-each (lambda (hfile)
		  (call-with-output-file hfile
		    (lambda (port)
		      (generate-C-layout-definitions port
						     hfile
						     include-check))))
		(cdr (assq 'hfiles alist))))))

;  $Log: layout.scm,v $
;  Revision 1.1.1.1  2001/11/05 17:47:21  tneale
;  Tornado shuffle
;
;  Revision 1.3  2001/01/19 22:22:35  paul
;  Update copyright.
;
;  Revision 1.2  1998/02/25 04:54:24  sra
;  Update copyrights.
;
;  Revision 1.1  1997/09/26 20:14:39  alan
;  Initial Revision
;
;  At this point, we migrated from Scheme->C to scsh
;  
;  Revision 1.17  1997/06/03 22:49:35  alan
;  Add support for `define-rcs-info' and boilerplate printing.
;
;  Revision 1.16  1997/06/03 02:35:18  alan
;  Add `define-layout-macro' so people can define macros in the layout
;  language.
;
;  Revision 1.15  1997/05/10 20:44:36  alan
;  Add `-include-check'.
;
;  Revision 1.14  1995/06/08 18:42:43  alan
;  Sort enumeration values.
;
;  Revision 1.13  1995/06/05  19:51:21  alan
;  Totally new output format for -makesch files.  -makeoldsch-makeoldsch gets
;  the previous format.  Less error checking of littlendian scalars -- a
;  better theory is needed.
;
;  Revision 1.12  1995/05/25  23:51:32  alan
;  Types have attribues.  Enums have a `base' attribute that controls how
;  their constants look in a `.h' file.
;
;  Revision 1.11  1995/05/17  22:15:03  alan
;  New scheme: .sch files include each other using the directory name
;
;  Revision 1.10  1995/05/09  04:33:13  alan
;  Be more careful about 24 bit cases.
;
;  Revision 1.9  1995/04/24  18:16:34  alan
;  Fix "can't" comment.
;
;  Revision 1.8  1995/04/19  08:01:42  alan
;  Seems to be a working .h file generator.
;  Still needs more error checking.
;  Only opaque and little-endian of size 8, 16 and 32 actually work.
;
;  Revision 1.7  1995/04/09  11:53:11  alan
;  First working command line version.
;
;  Revision 1.6  1995/04/06  23:19:46  alan
;  `generate-s2c-layout-definitions' now works
;
;  Revision 1.5  1995/04/05  21:19:23  alan
;  More work on `generate-scheme-layout-definitions'.
;
;  Revision 1.4  1995/04/03  07:55:55  alan
;  Still undebugged...
;
;  Revision 1.3  1995/03/31  21:52:38  alan
;  First crack at output driver loop -- undebugged
;
;  Revision 1.2  1995/03/29  01:09:02  alan
;  Tweak format.  Redefinition warnings.
;  Slot for base type.  Boolean type.
;
;  Revision 1.1  1995/03/28  19:42:00  alan
;  Initial revision

; Local Variables:
; mode: Scheme
; eval: (put 'sort-list! 'scheme-indent-hook 1)
; eval: (put 'generate-layout 'scheme-indent-hook 0)
; eval: (put 'for-each-boilerplate-line 'scheme-indent-hook 0)
; eval: (put 'with-interaction-environment 'scheme-indent-hook 1)
; End:
