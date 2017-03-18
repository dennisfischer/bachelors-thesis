@echo off
lualatex -synctex=1 -shell-escape -interaction=nonstopmode -output-directory=build thesis.tex
biber build/thesis.bcf
makeglossaries -d build thesis
lualatex -synctex=1 -shell-escape -interaction=nonstopmode -output-directory=build thesis.tex