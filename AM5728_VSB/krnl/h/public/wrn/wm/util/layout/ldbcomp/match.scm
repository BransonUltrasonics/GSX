; $Header: /usr/cvsroot/target/h/wrn/wm/util/layout/ldbcomp/match.scm,v 1.1.1.1 2001/11/05 17:47:21 tneale Exp $
; Simple matcher

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

(define (match? pat x)
  (cond ((pair? pat)
	 ((case (car pat)
	    ((quote) match-quote)
	    ((?) match-?)
	    ((*) match-*)
	    ((+) match-+)
	    ((or) match-or)
	    ((and) match-and)
	    ((not) match-not)
	    (else match-pair))
	  pat x))
	((symbol? pat)
	 (case pat
	   ((any anything) #T)
	   ((symbol) (symbol? x))
	   ((string) (string? x))
	   ((symbol/string) (or (symbol? x) (string? x)))	; very common
	   ((number) (number? x))
	   ((integer) (integer? x))
	   ((char) (char? x))
	   ((pair) (pair? x))
	   ((list) (list? x))
	   ((boolean) (boolean? x))
	   ((vector) (vector? x))
	   ;; ((record) (record? x))
	   (else (err 'match "Bad pattern: ~S" pat))))
	((procedure? pat) (pat x))
	(else (eqv? pat x))))

(define (match-quote pat x)
  (equal? (cadr pat) x))

(define (match-? pat x)
  (or (and (pair? x)
	   (match? (cadr pat) (car x))
	   (match? (cddr pat) (cdr x)))
      (match? (cddr pat) x)))

(define (match-* pat x)
  (let ((p1 (cadr pat))
	(p2 (cddr pat)))
    (let loop ((x x))
      (or (match? p2 x)
	  (and (pair? x)
	       (match? p1 (car x))
	       (loop (cdr x)))))))

(define (match-+ pat x)
  (and (pair? x)
       (match? (cadr pat) (car x))
       (match-* pat (cdr x))))

(define (match-pair pat x)
  (and (pair? x)
       (match? (car pat) (car x))
       (match? (cdr pat) (cdr x))))

(define (match-or pat x)
  (let loop ((l (cdr pat)))
    (and (not (null? l))
	 (or (match? (car l) x)
	     (loop (cdr l))))))

(define (match-and pat x)
  (let loop ((l (cdr pat)))
    (or (null? l)
	(and (match? (car l) x)
	     (loop (cdr l))))))

(define (match-not pat x)
  (not (match? (cadr pat) x)))

; $Log: match.scm,v $
; Revision 1.1.1.1  2001/11/05 17:47:21  tneale
; Tornado shuffle
;
; Revision 1.3  2001/01/19 22:22:35  paul
; Update copyright.
;
; Revision 1.2  1998/02/25 04:54:29  sra
; Update copyrights.
;
; Revision 1.1  1997/09/26 20:14:40  alan
; Initial Revision
;
; Local Variables:
; mode: Scheme
; End:
