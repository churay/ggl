# input output,indir,ext: outputs the path of the source file for the given output
inputs = $(shell find $(2) -name '$(basename $(notdir $(1)))$(3)')

# outputs inputs,outdir,ext: outputs the paths of the output files for the given inputs
outputs = $(patsubst %$(suffix $(word 1,$(1))),$(2)/%$(3),$(notdir $(1)))
