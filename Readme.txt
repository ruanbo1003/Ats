cd /home/ruanbo/Codes/RuboServer/msgs/protocols/
protoc --cpp_out=./ --python_out=/home/ruanbo/Clients/PyClients/TcpClients login.proto


valgrind --leak-check=full --track-origins=yes --log-file=mem.log ./Debug/Ats

valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=mem.log ./Debug/Ats



valgrind --leak-check=full --track-origins=yes --log-file=mem.log ./Debug/RuboServer  --gate



