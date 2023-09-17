; $Header: /usr/cvsroot/target/h/wrn/wm/util/layout/ldbcomp/dumpit.scm,v 1.1.1.1 2001/11/05 17:47:21 tneale Exp $
; Dump out an excutable for the layout tool.

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

(let ((target (nth command-line-arguments 0))
      (image (nth command-line-arguments 1)))
  (let* ((psarg (format #F "ww~A" (pid)))
	 (scshvm (symbol->string (nth (run/sexps (ps ,psarg)) 11)))
	 (lscshvm (string-length scshvm)))
    (unless (and (char=? #\/ (string-ref scshvm 0))
		 (string=? "/scshvm"
			   (substring scshvm (- lscshvm 7) lscshvm)))
      (err 'dumpit "Bad scshvm pathname: ~S" scshvm))
    (set! user-layouts-package (get-package 'user-layouts))
    (call-with-output-file target
      (lambda (port)
	(display "#!/bin/sh -" port)
	(newline port)
	(display scshvm port)
	(display " -i " port)
	(display image port)
	(display " \"$@\"" port)
	(newline port)
	))
    (set-file-mode target #o755)
    (set-err-should-exit? #T)
    (dump-scsh-program do-layout-command-line image)))

; $Log: dumpit.scm,v $
; Revision 1.1.1.1  2001/11/05 17:47:21  tneale
; Tornado shuffle
;
; Revision 1.3  2001/01/19 22:22:35  paul
; Update copyright.
;
; Revision 1.2  1998/02/25 04:54:20  sra
; Update copyrights.
;
; Revision 1.1  1997/09/26 20:14:39  alan
; Initial Revision
;
; Local Variables:
; mode: Scheme
; End:
