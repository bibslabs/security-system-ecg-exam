import http.server, ssl
import socket, socketserver
server_address = ('192.168.169.106', 4443)
httpd = http.server.HTTPServer(server_address, http.server.SimpleHTTPRequestHandler)
httpd.socket = ssl.wrap_socket(httpd.socket,
                               server_side=True,
                               certfile='cert.pem',
                               ssl_version=ssl.PROTOCOL_TLS)

httpd.serve_forever()
