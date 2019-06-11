#!/usr/bin/python3

from lxml import etree
import subprocess, shutil, sys
import os

SVG_DPI = 96.0
MM_PER_IN = 25.4
def mm2px(millimeters):
	return millimeters * (SVG_DPI / MM_PER_IN)

panel_height = 380

# Set input and output file name
input_file_name = "fb15.org.svg"
output_file_name = "fb15.svg"

# create initial output file
shutil.copy("res/" + input_file_name, "res/" + output_file_name)

# Get text and desc elements from the original document
input_file = open("res/" + input_file_name)
tree = etree.parse(input_file)
texts = tree.getroot().xpath("//*[local-name() = $name]", name="text")
descs = tree.getroot().xpath("//*[local-name() = $name]", name="desc")
input_file.close()

# Use Inkscape to convert each text element to a path and create header file
header_file = open("src/" + output_file_name + ".hpp", "w")
command = ["inkscape"]
for t in texts:
	command += ["--verb=EditDeselect", "--select=" + t.get("id"), "--verb=ObjectToPath"]
for desc in descs:
	lines = desc.text.split("\n")
	define = ""
	for line in lines:
		items = line.split(" ")
		name = items[0]
		if name == "name":
			define = items[1]
		if name == "ofs":
			ofsx = float(items[1])
			ofsy = float(items[2])
	if len(define) > 0:
		parent = desc.getparent()
		cx = float(parent.get("cx"))
		cy = float(parent.get("cy"))
		header_file.write("#define %s_POS Vec(%f, %f)\n" % (define, mm2px(cx + ofsx), mm2px(cy + ofsy) - panel_height))
		command += ["--verb=EditDeselect", "--select=" + parent.get("id"), "--verb=EditDelete"]
header_file.close()
command += ["--verb=FileSave", "--verb=FileClose", "--verb=FileQuit", "res/" + output_file_name]
subprocess.call(command)
