.PHONY: all clean coverity cov-clean

all: rmdir

clean: cov-clean
	rm -f rmdir

cov-clean:
	rm -f coverity_tool.tgz
	rm -f rmdir.tgz
	rm -rf cov-analysis-*
	rm -rf cov-int

rmdir: rmdir.c
	gcc -O2 -pipe -o $@ $^

coverity: clean
	wget https://scan.coverity.com/download/linux64 --post-data "token=J-MWZ3-yfX2zB51s_J3PfQ&project=vikman90%2Frmdir" -O coverity_tool.tgz
	tar -zxf coverity_tool.tgz
	cov-analysis-*/bin/cov-build --dir cov-int make
	tar czvf rmdir.tgz cov-int
	curl --form token=J-MWZ3-yfX2zB51s_J3PfQ \
	--form email=victor@wazuh.com \
	--form file=@rmdir.tgz \
	--form version="1.0" \
	--form description="Description" \
	https://scan.coverity.com/builds?project=vikman90%2Frmdir
