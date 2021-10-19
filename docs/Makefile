SHELL = /opt/homebrew/bin/fish

MDBIN = mdbin
IMGBIN = img/

PATH_CONV = sed -e 's|$(IMGBIN)|$(MDBIN)/$(IMGBIN)|g'
REPORT_MD = report.md
BACKUP_MD = report_backup.md
REPORT_HTML = report.html

report:
	@test -e $(REPORT_MD) && mv $(REPORT_MD) $(BACKUP_MD) || true
	cat $(MDBIN)/header.md	| $(PATH_CONV) 	 > $(REPORT_MD)
	@echo -e '\n---\n' 		>> $(REPORT_MD)
	cat $(MDBIN)/exp1.md	| sed -e 's|$(IMGBIN)|$(MDBIN)/$(IMGBIN)|g'	>> $(REPORT_MD)
	@echo -e '\n---\n' 		>> $(REPORT_MD)
	cat $(MDBIN)/exp2.md 	| sed -e 's|$(IMGBIN)|$(MDBIN)/$(IMGBIN)|g'	>> $(REPORT_MD)
	@echo -e '\n---\n' 		>> $(REPORT_MD)
	cat $(MDBIN)/end.md		| sed -e 's|$(IMGBIN)|$(MDBIN)/$(IMGBIN)|g'	>> $(REPORT_MD)

html: report
	md2html $(REPORT_MD) > $(REPORT_HTML)

