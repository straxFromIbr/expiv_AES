SHELL = /opt/homebrew/bin/fish

MDBIN = mdsrc
IMGBIN = img/

PATH_CONV = sed -e 's|$(IMGBIN)|$(MDBIN)/$(IMGBIN)|g'
REPORT_MD = report.md
BACKUP_MD = report_backup.md
REPORT_HTML = report.html

report:
	@test -e $(REPORT_MD) && mv $(REPORT_MD) $(BACKUP_MD) || true
	@echo > $(REPORT_MD)
	for mdf in $(MDBIN)/*.md ;\
		cat $$mdf | $(PATH_CONV) >> $(REPORT_MD);\
		echo -e '\n\n---\n\n' >> $(REPORT_MD);\
	end

html: report
	md2html $(REPORT_MD) > $(REPORT_HTML)

