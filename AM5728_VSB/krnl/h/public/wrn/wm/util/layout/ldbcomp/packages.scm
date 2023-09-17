; $Header: /usr/cvsroot/target/h/wrn/wm/util/layout/ldbcomp/packages.scm,v 1.1.1.1 2001/11/05 17:47:21 tneale Exp $
; Package definitions for the layout tool.

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

(define-structure utils
  (export (defmac :syntax)
	  (comment :syntax)
	  (define-constant :syntax)
	  (define-in-line :syntax)
	  (define-integrable :syntax)
	  (when :syntax) (unless :syntax)
	  set-err-should-exit?
	  err
	  make-string-hash-table
	  make-symbol-hash-table
	  hash-table/lookup
	  hash-table/get hash-table/put! hash-table/modify!
	  hash-table/clear! hash-table/for-each
	  parse-options
	  )
  ;; Eventually weed some of these out...
  (open defrec-package
	scsh		; order matters
	;;signals
	;;conditions
	;;handle
	tables
	;;sort
	scheme
	)
  (files utils
	 ))

(define-structure lists
  (export last-pair cons*
	  reverse! reverse*! reverse* append!
	  map* append-map append-map* append-map! append-map*!
	  there-exists? for-all?
	  reduce reduce-right reduce-map reduce-right-map
	  fold fold* fold-map fold-map*
	  fold-reverse fold-reverse* fold-reverse-map fold-reverse-map*
	  )
  (open utils
	scheme
	)
  (files lists
	 ))

(define-structure strings
  (export string-search-forwards string-ci-search-forwards
	  string-search-forwards-not string-ci-search-forwards-not
	  string-search-backwards string-ci-search-backwards
	  string-search-backwards-not string-ci-search-backwards-not
	  string-downcase string-downcase!
	  string-upcase string-upcase!
	  string-capitalize string-capitalize!
	  pathname-entry-name pathname-directory-name
	  pathname-append
	  pathname-relative?
	  pathname->list
	  list->pathname
	  canonicalize-pathname-list
	  canonicalize-pathname
	  symbol-append
	  encode-integer-in-base
	  )
  (open lists
	utils
	scheme
	)
  (files strings
	 ))

(define-structure match (export match?)
  (open utils
	scheme
	)
  (files match
	 ))

(define-structure layout-language (export (define-layout :syntax)
					  (define-layout-macro :syntax)
					  define-rcs-info
					  use-layouts
					  (C-include :syntax)
					  )
  (open match
	strings
	lists
	utils
	defrec-package
	scsh		; order matters
	sort
	evaluation			; Ick
	environments			; Yuck!
	package-commands-internal	; Gag!!
	scheme
	)
  (files layout
	 ))

; This is the package we actually load .ldb files into:
(define-structure user-layouts (export)
  (open layout-language
	scheme
	))

; $Log: packages.scm,v $
; Revision 1.1.1.1  2001/11/05 17:47:21  tneale
; Tornado shuffle
;
; Revision 1.3  2001/01/19 22:22:36  paul
; Update copyright.
;
; Revision 1.2  1998/02/25 04:54:31  sra
; Update copyrights.
;
; Revision 1.1  1997/09/26 20:14:41  alan
; Initial Revision
;
; Local Variables:
; mode: Scheme
; End:
