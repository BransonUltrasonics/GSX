; $Header: /usr/cvsroot/target/h/wrn/wm/util/layout/ldbcomp/utils.scm,v 1.1.1.1 2001/11/05 17:47:21 tneale Exp $
; Utilities for the layout tool.

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

;
; Macrology
;

(define-syntax defmac
  (syntax-rules ()
    ((defmac (name . bvl) def)
     (define-syntax name
       (syntax-rules ()
	 ((name . bvl) def))))))

(defmac (comment . stuff)
  (quote (comment)))

(defmac (define-constant . stuff)
  (define . stuff))

(defmac (define-in-line (name . bvl) expr)
  (define-syntax name
    (syntax-rules ()
      ((name . bvl) expr))))

(defmac (define-integrable . stuff)
  (define . stuff))

(defmac (when test first . rest)
  (if test (begin first . rest)))

(defmac (unless test first . rest)
  (if (not test) (begin first . rest)))

;
; Errors
;

(define err-should-exit? #F)

(define (set-err-should-exit? flag)
  (set! err-should-exit? flag))

(define (err id format-string . format-args)
  (format (error-output-port) "layout: ")
  (unless (eq? id 'layout)
    (format (error-output-port) "~A: " id))
  (apply format (error-output-port) format-string format-args)
  (cond (err-should-exit?
	 (newline (error-output-port))
	 (exit 1))
	(else
	 (apply error format-string format-args))))

;
; (Hash) Tables
;

(define-record hash-table
  table
  init
  )

(define (make-string-hash-table initial-size)
  (make-hash-table (make-string-table) make-string-table))

(define (make-symbol-hash-table initial-size)
  (make-hash-table (make-symbol-table) make-symbol-table))

(define (hash-table/clear! table)
  (set-hash-table:table table ((hash-table:init table))))

(define (hash-table/get table key default)
  (let ((cell (table-ref (hash-table:table table) key)))
    (if cell (car cell) default)))

(define (hash-table/put! table key value)
  (let* ((table (hash-table:table table))
	 (cell (table-ref table key)))
    (if cell
	(set-car! cell value)
	(table-set! table key (list value)))))

(define (hash-table/modify! table key default proc)
  (let* ((table (hash-table:table table))
	 (cell (table-ref table key)))
    (if cell
	(set-car! cell (proc (car cell)))
	(table-set! table key (list (proc default))))))

(define (hash-table/lookup table key win lose)
  (let ((cell (table-ref (hash-table:table table) key)))
    (if cell
	(win (car cell))
	(lose))))

(define (hash-table/for-each table procedure)
  (table-walk (lambda (key cell)
		(procedure key (car cell)))
	      (hash-table:table table)))

;
; Operating system interface
;

(define (parse-options args specs)
  ;; each spec is of the form: ("-foo" <kind> foo)
  ;; "--" is understood
  ;; Possible <kind>s:
  ;; #F or FLAG		boolean flag
  ;; #T or ARG		next argument is the value
  ;; EXEC		arguments up to ";" are values
  ;;			(like "find ... -exec ... \;".  Yuck!)
  (let ((alist (let loop ((alist '())
			  (specs specs))
		 (if (null? specs)
		     alist
		     (case (cadr (car specs))
		       ((#F flag)
			(loop alist (cdr specs)))
		       ((arg #T exec)
			(loop (cons (cons (caddr (car specs)) '()) alist)
			      (cdr specs)))
		       (else
			(err 'parse-options "bad spec: ~S" (car specs))))))))
    (define (add prop vals)
      (let ((p (assq prop alist)))
	(if p
	    (set-cdr! p (append (cdr p) vals))
	    (set! alist (cons (cons prop vals) alist)))))
    (let loop ((args args))
      (if (null? args)
	  (add 'args '())
	  (let ((opt (car args)))
	    (if (and (<= 2 (string-length opt))
		     (char=? #\- (string-ref opt 0)))
		(let ((spec (assoc opt specs)))
		  (cond (spec
			 (case (cadr spec)
			   ((#F flag)
			    (add (caddr spec) '())
			    (loop (cdr args)))
			   ((#T arg)
			    (when (null? (cdr args))
			      (err 'parse-options
				   "option requires argument: ~A" opt))
			    (add (caddr spec) (list (cadr args)))
			    (loop (cddr args)))
			   ((exec)
			    (let l ((args (cdr args))
				    (vals '()))
			      (cond ((null? args)
				     (err 'parse-options
					  "missing `;': ~A" opt))
				    ((string=? ";" (car args))
				     (add (caddr spec) (list (reverse vals)))
				     (loop (cdr args)))
				    (else
				     (l (cdr args) (cons (car args) vals))))))
			   (else
			    (err 'parse-options
				 "unknown option type: ~A" (cadr spec)))))
			((string=? opt "--")
			 (add 'args (cdr args)))
			(else
			 (err 'parse-options "unknown option: ~A" opt))))
		(add 'args args)))))
    alist))

; $Log: utils.scm,v $
; Revision 1.1.1.1  2001/11/05 17:47:21  tneale
; Tornado shuffle
;
; Revision 1.3  2001/01/19 22:22:36  paul
; Update copyright.
;
; Revision 1.2  1998/02/25 04:54:36  sra
; Update copyrights.
;
; Revision 1.1  1997/09/26 20:14:41  alan
; Initial Revision
;
; Local Variables:
; mode: Scheme
; eval: (put 'hash-table/modify! 'scheme-indent-hook 3)
; eval: (put 'hash-table/lookup 'scheme-indent-hook 2)
; eval: (put 'hash-table/for-each 'scheme-indent-hook 1)
; End:
