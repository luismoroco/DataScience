APP=Task1

.PHONY: push
push:
	git add .
	git commit -m "Generated by Makefile"
	git push origin main

.PHONY: pull 
pull:
	git pull origin main