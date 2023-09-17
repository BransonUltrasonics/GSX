; $Header: /usr/cvsroot/target/h/wrn/wm/util/layout/ldbcomp/lists.scm,v 1.1.1.1 2001/11/05 17:47:21 tneale Exp $
; List manipulation, mapping, folding, reducing...

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

(define (last-pair l)
  (let loop ((cell l)
	     (next (cdr l)))
    (if (pair? next)
	(loop next (cdr next))
	cell)))

(define (cons* first . rest)
  (cond ((null? rest) first)
	((null? (cdr rest)) (cons first (car rest)))
	(else (let loop ((a rest)
			 (b (cdr rest))
			 (c (cddr rest)))
		(if (null? c)
		    (set-cdr! a (car b))
		    (loop b c (cdr c))))
	      (cons first rest))))

(define (reverse! lst)
  (reverse*! lst '()))

(define (reverse*! lst tail)
  (let loop ((in lst) (out tail))
    (cond ((null? in) out)
	  ((pair? in)
	   (let ((x (cdr in)))
	     (set-cdr! in out)
	     (loop x in)))
	  (else (err 'reverse*! "Not a proper list: ~S" in)))))

(define (reverse* lst tail)
  (let loop ((in lst) (out tail))
    (cond ((null? in) out)
	  ((pair? in) (loop (cdr in) (cons (car in) out)))
	  (else (err 'reverse* "Not a proper list: ~S" lst)))))

(define (append! lst tail)
  (cond ((pair? lst)
	 (let loop ((prev lst) (l (cdr lst)))
	   (if (pair? l)
	       (loop l (cdr l))
	       (set-cdr! prev tail)))
	 lst)
	(else tail)))

(define (map* tail proc lst)
  (let loop ((l lst))
    (cond ((null? l) tail)
	  ((pair? l)
	   (cons (proc (car l))
		 (loop (cdr l))))
	  (else (err 'map* "Not a proper list: ~S" lst)))))

(define (append-map proc lst) (append-map* '() proc lst))

(define (append-map* tail proc lst)
  (let loop ((l lst))
    (cond ((null? l) tail)
	  ((pair? l)
	   (append (proc (car l))
		   (loop (cdr l))))
	  (else (err 'append-map* "Not a proper list: ~S" lst)))))

(define (append-map! proc lst) (append-map*! '() proc lst))

(define (append-map*! tail proc lst)
  (let loop ((l lst))
    (cond ((null? l) tail)
	  ((pair? l)
	   (append! (proc (car l))
		    (loop (cdr l))))
	  (else (err 'append-map*! "Not a proper list: ~S" lst)))))

(define (there-exists? list predicate)
  (let loop ((list list))
    (and (not (null? list))
	 (or (predicate (car list))
	     (loop (cdr list))))))

(define (for-all? list predicate)
  (let loop ((list list))
    (or (null? list)
	(and (predicate (car list))
	     (loop (cdr list))))))

(define (reduce procedure initial list)
  (if (null? list)
      initial
      (let loop ((val (car list))
		 (l (cdr list)))
	(if (null? l)
	    val
	    (loop (procedure val (car l))
		  (cdr l))))))

(define (reduce-right procedure initial list)
  (if (null? list)
      initial
      (let loop ((first (car list))
		 (rest (cdr list)))
	(if (null? rest)
	    first
	    (procedure first (loop (car rest) (cdr rest)))))))

(define (reduce-map reduce-proc initial map-proc lst)
  (if (null? lst)
      initial
      (let loop ((val (map-proc (car lst)))
		 (l (cdr lst)))
	(if (null? l)
	    val
	    (loop (reduce-proc val (map-proc (car l))) (cdr l))))))

(define (reduce-right-map reduce-proc initial map-proc lst)
  (if (null? lst)
      initial
      (let loop ((val (map-proc (car lst)))
		 (l (cdr lst)))
	(if (null? l)
	    val
	    (reduce-proc val (loop (map-proc (car l)) (cdr l)))))))

(define (fold fold-proc lst)
  (fold* fold-proc '() lst))

(define (fold* fold-proc tail lst)
  (let loop ((l lst))
    (cond ((null? l) tail)
	  ((pair? l) (fold-proc (car l) (loop (cdr l))))
	  (else (err 'fold* "Not a proper list: ~S" lst)))))

(define (fold-map fold-proc map-proc lst)
  (fold-map* fold-proc '() map-proc lst))

(define (fold-map* fold-proc tail map-proc lst)
  (let loop ((l lst))
    (cond ((null? l) tail)
	  ((pair? l)
	   (fold-proc (map-proc (car l))
		      (loop (cdr l))))
	  (else (err 'fold-map* "Not a proper list: ~S" lst)))))

(define (fold-reverse fold-proc lst)
  (fold-reverse* fold-proc '() lst))

(define (fold-reverse* fold-proc tail lst)
  (let loop ((l lst)
	     (v tail))
    (cond ((null? l) v)
	  ((pair? l) (loop (cdr l) (fold-proc (car l) v)))
	  (else (err 'fold-reverse* "Not a proper list: ~S" lst)))))

(define (fold-reverse-map fold-proc map-proc lst)
  (fold-reverse-map* fold-proc '() map-proc lst))

(define (fold-reverse-map* fold-proc tail map-proc lst)
  (let loop ((l lst)
	     (v tail))
    (cond ((null? l) v)
	  ((pair? l) (loop (cdr l) (fold-proc (map-proc (car l)) v)))
	  (else (err 'fold-reverse-map* "Not a proper list: ~S" lst)))))

; $Log: lists.scm,v $
; Revision 1.1.1.1  2001/11/05 17:47:21  tneale
; Tornado shuffle
;
; Revision 1.3  2001/01/19 22:22:35  paul
; Update copyright.
;
; Revision 1.2  1998/02/25 04:54:27  sra
; Update copyrights.
;
; Revision 1.1  1997/09/26 20:14:40  alan
; Initial Revision
;
; Local Variables:
; mode: Scheme
; eval: (put 'for-all\? 'scheme-indent-hook 1)
; eval: (put 'there-exists\? 'scheme-indent-hook 1)
; End:
