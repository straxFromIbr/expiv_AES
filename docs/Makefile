SHELL = /opt/homebrew/bin/fish

MDBIN = mdsrc
IMGBIN = img/

PATH_CONV = sed -e 's|$(IMGBIN)|$(MDBIN)/$(IMGBIN)|g'
REPORT_MD = report.md
BACKUP_MD = report_backup.md
REPORT_HTML = report.html
REPORT_PDF = report.pdf

report:
	@echo > $(REPORT_MD)
	for mdf in (ls $(MDBIN)/*.md | sort) ;\
		cat $$mdf | $(PATH_CONV) >> $(REPORT_MD);\
		echo -e '\n\n---\n\n' >> $(REPORT_MD);\
	end

html: report
	md2html $(REPORT_MD) > $(REPORT_HTML)

pdf: html
	chrome \
		--headless\
		--run-all-compositor-stages-before-draw \
		--virtual-time-budget=10000 \
		--print-to-pdf \
		$(REPORT_HTML) && mv output.pdf $(REPORT_PDF)

clean:
	rm -f $(REPORT_HTML) $(REPORT_PDF) $(REPORT_MD)
