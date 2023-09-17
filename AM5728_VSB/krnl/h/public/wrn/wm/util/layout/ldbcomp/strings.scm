; $Header: /usr/cvsroot/target/h/wrn/wm/util/layout/ldbcomp/strings.scm,v 1.1.1.1 2001/11/05 17:47:21 tneale Exp $
; Strings and pathnames

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
; Case
;

(define (string-upcase str)
  (let* ((len (string-length str))
	 (new (make-string len)))
    (do ((i (- len 1) (- i 1)))
	((< i 0))
      (string-set! new i (char-upcase (string-ref str i))))
    new))

(define (string-downcase str)
  (let* ((len (string-length str))
	 (new (make-string len)))
    (do ((i (- len 1) (- i 1)))
	((< i 0))
      (string-set! new i (char-downcase (string-ref str i))))
    new))

(define (string-upcase! str)
  (do ((i (- (string-length str) 1) (- i 1)))
      ((< i 0))
    (string-set! str i (char-upcase (string-ref str i))))
  str)

(define (string-downcase! str)
  (do ((i (- (string-length str) 1) (- i 1)))
      ((< i 0))
    (string-set! str i (char-downcase (string-ref str i))))
  str)

(define (string-capitalize str)
  (let* ((len (string-length str))
	 (new (make-string len)))
    (let loop ((i 0)
	       (last-alpha? #F))
      (when (< i len)
	(let ((c (string-ref str i)))
	  (string-set! new i (if last-alpha?
				 (char-downcase c)
				 (char-upcase c)))
	  (loop (+ i 1) (char-alphabetic? c)))))
    new))

(define (string-capitalize! str)
  (let ((len (string-length str)))
    (let loop ((i 0)
	       (last-alpha? #F))
      (when (< i len)
	(let ((c (string-ref str i)))
	  (string-set! str i (if last-alpha?
				 (char-downcase c)
				 (char-upcase c)))
	  (loop (+ i 1) (char-alphabetic? c)))))
    str))

;
; Searching
;

; These all assume that you won't pass bogus values of `beg' and `end'.
; It is safe to compare characters using `eq?' in Scheme48...

(define (string-search-forwards chr str beg end)
  (let loop ((i beg))
    (cond ((>= i end) #F)
	  ((eq? chr (string-ref str i)) i)
	  (else (loop (+ i 1))))))

(define (string-search-forwards-not chr str beg end)
  (let loop ((i beg))
    (cond ((>= i end) #F)
	  ((not (eq? chr (string-ref str i))) i)
	  (else (loop (+ i 1))))))

(define (string-search-backwards chr str beg end)
  (let loop ((i (- end 1)))
    (cond ((< i beg) #F)
	  ((eq? chr (string-ref str i)) i)
	  (else (loop (- i 1))))))

(define (string-search-backwards-not chr str beg end)
  (let loop ((i (- end 1)))
    (cond ((< i beg) #F)
	  ((not (eq? chr (string-ref str i))) i)
	  (else (loop (- i 1))))))

; And the -ci versions:

(define (string-ci-search-forwards chr str beg end)
  (let loop ((i beg))
    (cond ((>= i end) #F)
	  ((char-ci=? chr (string-ref str i)) i)
	  (else (loop (+ i 1))))))

(define (string-ci-search-forwards-not chr str beg end)
  (let loop ((i beg))
    (cond ((>= i end) #F)
	  ((not (char-ci=? chr (string-ref str i))) i)
	  (else (loop (+ i 1))))))

(define (string-ci-search-backwards chr str beg end)
  (let loop ((i (- end 1)))
    (cond ((< i beg) #F)
	  ((char-ci=? chr (string-ref str i)) i)
	  (else (loop (- i 1))))))

(define (string-ci-search-backwards-not chr str beg end)
  (let loop ((i (- end 1)))
    (cond ((< i beg) #F)
	  ((not (char-ci=? chr (string-ref str i))) i)
	  (else (loop (- i 1))))))

;
; Pathnames
;

(define (pathname-entry-name pathname)
  (let* ((len (string-length pathname))
	 (j (string-search-backwards-not #\/ pathname 0 len)))
    (if (not j)
	"."
	(let ((i (string-search-backwards #\/ pathname 0 j)))
	  (if (not i)
	      (substring pathname 0 (+ 1 j))
	      (substring pathname (+ 1 i) (+ 1 j)))))))

(define (pathname-directory-name pathname)
  ;; Always ends with a "/", both for convenience in concatenation, and so
  ;; that the root directory has a non-empty name.
  (let* ((len (string-length pathname))
	 (j (string-search-backwards-not #\/ pathname 0 len)))
    (if (not j)
	"/"
	(let ((i (string-search-backwards #\/ pathname 0 j)))
	  (if (not i)
	      "./"
	      (let ((h (string-search-backwards-not #\/ pathname 0 i)))
		(if (not h)
		    "/"
		    (substring pathname 0 (+ h 2)))))))))

(define (pathname-append pn1 pn2)
  (let ((len1 (string-length pn1))
	(len2 (string-length pn2)))
    (cond ((= 0 len1) pn2)
	  ((= 0 len2) pn1)
	  ((eq? #\/ (string-ref pn2 0)) pn2)
	  ((eq? #\/ (string-ref pn1 (- len1 1)))
	   (string-append pn1 pn2))
	  (else (string-append pn1 "/" pn2)))))

(define (pathname-relative? pn)
  (or (= 0 (string-length pn))
      (not (eq? #\/ (string-ref pn 0)))))

(define (pathname->list pathname)
  (let ((end (string-length pathname)))
    (if (= 0 end)
	'(relative)
	(cons (if (eq? #\/ (string-ref pathname 0))
		  'absolute
		  'relative)
	      (let loop ((beg 0))
		(let ((i (string-search-forwards-not #\/ pathname beg end)))
		  (if (not i)
		      '()
		      (let ((j (string-search-forwards #\/ pathname i end)))
			(if (not j)
			    (list (substring pathname i end))
			    (cons (substring pathname i j)
				  (loop j)))))))))))

(define (list->pathname lst)
  (define (process lst)
    (if (null? lst)
	'()
	(cons* "/" (car lst) (process (cdr lst)))))
  (case (car lst)
    ((absolute)
     (if (null? (cdr lst))
	 "/"
	 (apply string-append (process (cdr lst)))))
    ((relative)
     (if (null? (cdr lst))
	 "."
	 (apply string-append (cdr (process (cdr lst))))))
    (else (err 'list->pathname "Bad pathname list: ~S" lst))))

(define (canonicalize-pathname-list lst)
  (let loop ((l (cdr lst))
	     (p '())
	     (r '()))
    (cond ((null? l)
	   (cons (car lst) (append! p (reverse! r))))
	  ((string=? "." (car l))
	   (loop (cdr l) p r))
	  ((string=? ".." (car l))
	   (if (null? r)
	       (loop (cdr l) (cons ".." p) r)
	       (loop (cdr l) p (cdr r))))
	  (else
	   (loop (cdr l) p (cons (car l) r))))))

(define (canonicalize-pathname pathname)
  (list->pathname (canonicalize-pathname-list (pathname->list pathname))))

;
; Making things into strings
;
(define (symbol-append . args)
  (define (->string arg)
    (cond ((string? arg)  arg)
	  ((symbol? arg) (symbol->string arg))
	  ((char? arg) (string arg))
	  ((and (integer? arg) (exact? arg))
	   (number->string arg 10))
	  (else (err 'symbol-append "Don't know how to use: ~S" arg))))
  (string->symbol (apply string-append (map ->string args))))

(define (encode-integer-in-base n base digits)
  (define (loop n l)
    (if (< n base)
	(cons (string-ref digits n) l)
	(loop (quotient n base)
	      (cons (string-ref digits (modulo n base)) l))))
  (unless (integer? n)
    (err 'encode-integer-in-base "Not an integer: ~S" n))
  (unless (and (integer? base) (> base 1))
    (err 'encode-integer-in-base "Bad base: ~S" base))
  (unless (<= base (string-length digits))
    (err 'encode-integer-in-base "Not enough digits for base ~S: ~S"
	 base digits))
  (list->string (if (< n 0)
		    (cons #\- (loop (- n) '()))
		    (loop n '()))))

; $Log: strings.scm,v $
; Revision 1.1.1.1  2001/11/05 17:47:21  tneale
; Tornado shuffle
;
; Revision 1.3  2001/01/19 22:22:36  paul
; Update copyright.
;
; Revision 1.2  1998/02/25 04:54:33  sra
; Update copyrights.
;
; Revision 1.1  1997/09/26 20:14:41  alan
; Initial Revision
;
; Local Variables:
; mode: Scheme
; End:
