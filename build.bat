@echo off
rem pdflatex -synctex=1 -shell-escape -interaction=nonstopmode -output-directory=build thesis.tex
rem biber build/thesis.bcf
makeglossaries -d build thesis
rem pdflatex -synctex=1 -shell-escape -interaction=nonstopmode -output-directory=build thesis.tex