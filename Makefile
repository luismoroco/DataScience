APP=Task1

.PHONY: push
push:
	git add .
	git commit -m "Makefile generated"
	git push origin main