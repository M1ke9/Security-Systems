all: pcap_ex

pcap_ex: pcap_ex.c 
	gcc pcap_ex.c -o pcap_ex -lpcap

run: pcap_ex
	./pcap_ex

clean:
	rm -rf pcap_ex log.txt