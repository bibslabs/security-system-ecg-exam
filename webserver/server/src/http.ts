import express, { json, request, response } from 'express';
import { createServer } from 'http';
import { Server, Socket } from 'socket.io';
import cors from 'cors';
import { open, close,write } from 'fs';

import './database';
import { routes } from './routes';
import { ExclusionMetadata } from 'typeorm/metadata/ExclusionMetadata';
import { ReadStream } from 'typeorm/platform/PlatformTools';
var microtime = require('microtime')
const NTP = require('ntp-time').Client;
const client = new NTP('pool.ntp.br', 123, { timeout: 5000 });
var time_diff:number = 0

//GMT -3 offset in micros
// const micro_offset = 10800000000

function get_clock_diffs(ntp_time){
	time_diff = Date.now() - ntp_time;
	console.log("Time drift is -> ",time_diff);
}

function calc_latency(received_timestamp){
	const latency = (Date.now() - time_diff) - received_timestamp;
	// console.log("Latency is -> ", latency, " milliseconds")
	return latency
}

function calc_latency_hr(received_timestamp){
	const latency = microtime.now() - Number(received_timestamp)
	return Number(latency)
}

client
	.syncTime()
	.then(response => get_clock_diffs(Math.floor(response["d"]*1000)))
	.catch(console.log);

//values to check every second
var packets:number = 0;
var bytes:number = 0;
var ticks:number = 0;
var latencies:number[] = []

class test_procedure{
	algo: string;
	pkg_s: string;
}

class test_result{
	min : number;
	max : number;
	speeds: number[];
	packets: number[];
	lats: number[];
}

var algos: string[] = ["NONE","AESCBC","SPECK","CLEFIA"]

var tests: test_procedure[] = [
{algo:"NONE",pkg_s:"100"},{algo:"AESCBC",pkg_s:"170"},{algo:"AESCBC",pkg_s:"190"},{algo:"AESCBC",pkg_s:"210"},{algo:"AESCBC",pkg_s:"230"},{algo:"AESCBC",pkg_s:"250"},
{algo:"SPECK",pkg_s:"270"},{algo:"SPECK",pkg_s:"290"},{algo:"SPECK",pkg_s:"300"},{algo:"SPECK",pkg_s:"140"},{algo:"SPECK",pkg_s:"150"},{algo:"SPECK",pkg_s:"160"},
{algo:"CLEFIA",pkg_s:"170"},{algo:"CLEFIA",pkg_s:"190"},{algo:"CLEFIA",pkg_s:"200"},{algo:"CLEFIA",pkg_s:"210"},{algo:"CLEFIA",pkg_s:"220"},{algo:"CLEFIA",pkg_s:"230"},
]

var current_procedre:number = 0;
var interval;
var cur_algo:number = 0
var esp_list: esp_unit[] = []
var results: test_result[] = []
const min_samples = 100;
const max_samples = 1000;
const sample_increase = 10

const max_ticks:number = 60;

var current_sample_size = min_samples;


function set_esp_config(value:test_procedure){
	esp_list.forEach(function(data){
		if(data.state == ESP_STATE.ESP_SEND){
			data.socket.emit("test-cfg",value);
		}
	});
}

const average = arr => arr.reduce((a,b) => a + b, 0) / arr.length;

var current_test:test_result = {min:0,max:0,speeds:[],packets:[],lats:[]};

function throughput_test() {
	if(ticks < max_ticks){
		console.log("Throughput test -> " + bytes + " bytes por segundo em " + packets + " pacotes" + "com latencia media" + average(latencies));
		console.log()
		current_test.speeds.push(bytes);
		current_test.packets.push(packets);
		packets = 0;
		bytes = 0;
		latencies = []
		ticks+=1;
	}else{
		console.log("Gravando teste |  algoritmo " + algos[cur_algo] + " amostras " + current_sample_size);
		current_test.max = Math.max(...current_test.speeds)//operador spread ... faz algum milagre
		current_test.min = Math.min(...current_test.speeds)
		current_test.lats = latencies
		results.push(current_test);
		write_result(current_test);
		// current_test.speeds = [];
		// current_test.packets = [];
		current_sample_size += sample_increase
		if(current_sample_size > max_samples){
			cur_algo++;
			current_sample_size = min_samples;
		}
		const change_test:test_procedure = {algo:algos[cur_algo],pkg_s:current_sample_size.toString()}
		set_esp_config(change_test);
		ticks = 0;
		current_test = {min:0,max:0,speeds:[],packets:[],lats:[]};
	}
}


function write_result(json_object){
	const path = require('path');
	const algo = algos[cur_algo];
	const pkg_s = current_sample_size.toString();
	const absPath = path.join(__dirname,"testresults",algo + "-"+ pkg_s + ".txt");
	open(absPath, 'w', (err, fd) => {
		if (err) {
		  if (err.code === 'EEXIST') {
			console.error('myfile already exists');
			return;
		  }
	  
		  throw err;
		}
	  
		try {
		  write(fd,JSON.stringify(json_object,null,2),function(err,written){

		  });
		} finally {
		  close(fd, (err) => {
			if (err) throw err;
		  });
		}
	  });
}

class esp_unit {
	state : ESP_STATE;
	key : string;
	algo : string;
	mac : string;
	ip : string;
	socket: Socket;
}




//estados do esp
enum ESP_STATE {
	ESP_BEGIN = 0, //recem conectou
	ESP_AUTH = 1, //recebeu a chave
	ESP_SEND = 2, //respondeu que esta pronto para comecar
}

function request_decrypt(algo : string, key : string, data_b64 : string) {
	const path = require("path");
  
	var input = {
	  data:data_b64,
	  algo:algo,
	  key:key,
	  mode:"decrypt"
	}
	const input_str = JSON.stringify(input)
	console.log(input_str);
	const execPath = path.resolve(__dirname,"./crypto.exe");
	var spawn = require('child_process').spawnSync;
	var child = spawn(execPath, [input_str]);
	const res =  Buffer.from(child.stderr,"base64").toString("ascii");
	console.log(res)
	const ret = JSON.parse(res);
	return ret["result"];
  }
  
function create_new_esp(ip : string, sock : Socket) : any {
	const new_esp: esp_unit = {
		ip: ip,
		state:ESP_STATE.ESP_BEGIN,
		key: "",
		mac: "",
		algo:"",
		socket:sock
	};
	return new_esp;
}

function create_esp_key(device : esp_unit): any {
	// esp_list[achei].state = ESP_STATE.ESP_AUTH;
	var crypto = require("crypto");
	const crypto_key = crypto.randomBytes(16).toString('hex');
	const crypt = {
		key : crypto_key,
		algo : "AESCBC"
	}
	device.key = crypto_key;
	device.algo = crypt.algo;
	device.state = ESP_STATE.ESP_AUTH;
	console.log("Chave gerada para ESP");
	console.log(crypto_key);
	return crypt;
}



const app = express();

const http = createServer(app); //criando protocolo http
// const io = new Server(http); //criando protocolo websocket

const io = require('socket.io')(http, {
	cors: {
		origin: 'http://192.168.169.106:3000',
		methods: ['GET', 'POST'],
		allowedHeaders: ['my-custom-header'],
		credentials: true,
	},
});

app.use(cors());
// TODO: ver como fazer esse put para atualizar o dado da tabela sempre que atualizar o socket id

// const socket_client = [];
io.on('connection', (socket: Socket) => {
	// socket_client.push({
	// 	user_id: socket.id,
	// });
	
	console.log('socket criado no http', socket.id);
	//procura o esp
	const ip = socket.handshake.address; // ip do esp
	const achei = esp_list.findIndex(data => data.ip  === ip);

	//-1 se nao encontrado, >= 0 retorna o indice
	if(achei >= 0){
		console.log("Ja registrado!");
		esp_list[achei].socket = socket;
	}else{
		esp_list.push(create_new_esp(ip,socket))
		console.log("esp registrado")
	}

	socket.emit('message', 'hello14');
	socket.on('hello', (arg) => {
		console.log(arg); // world
	});

	socket.on('client-socket', (id) => {
		console.log('id do socket client', id);
	});
	//mensagem inicial
	socket.on('esp-message', (arg) => {
		console.log('arg:', arg); // message esp
		socket.broadcast.emit('client-message', arg);
		//procura o esp na lista ja registrado
		const ip = socket.handshake.address; // ip do esp
		const achei = esp_list.findIndex(data => data.ip  === ip);
		if(achei == -1){
			return
		}
		
		switch(esp_list[achei].state){
			case ESP_STATE.ESP_BEGIN: {
				console.log("Esp em estado inicial");
				//registra esp com chaves e algoritmo e 
				socket.emit("set-crypt",create_esp_key(esp_list[achei]))
				break;
			}
			case ESP_STATE.ESP_AUTH: {
				console.log("Esp autenticado");
				break;
			}
		}
	});

	//esp enviara uma mensagem de que esta pront
    //esta mensagem sera descriptografada para 
	//validar que ambos estao comunicando com a mesma chave e algoritmo
	socket.on('crypto-set', (arg) => {
		console.log(arg);
		//procura o esp na lista ja registrado
		const ip = socket.handshake.address; // ip do esp
		const achei = esp_list.findIndex(data => data.ip  === ip);
		// console.log(esp_list[achei]);
		if(esp_list[achei].state == ESP_STATE.ESP_AUTH){
			console.log('arg:', arg); // message esp
			var res = request_decrypt(esp_list[achei].algo,esp_list[achei].key,arg);
			const res_str  = Buffer.from(res,"base64").toString("ascii");
			if(res_str == "Ready to start!"){
				console.log("lets go!");
				socket.emit("throughput-start","");
				interval = setInterval(throughput_test, 1000);
				esp_list[achei].state = ESP_STATE.ESP_SEND;
				socket.emit("test-cfg",tests[current_procedre]);
				current_sample_size = 100
				cur_algo = 3
			}
		}else{
			console.log("Reset evenviado!")
			socket.emit("reset","");
		}
		
		
		// socket_client.forEach(function (item) {
		// });
	});

	socket.on('throughput', (arg,arg2) => {
		//ntp time
		// client.
		const lat = calc_latency_hr(parseInt(arg2))
		const ip = socket.handshake.address; // ip do esp
		// console.log(lat)
		const achei = esp_list.findIndex(data => data.ip  === ip);
		// console.log(esp_list[achei]);
		if(esp_list[achei].state == ESP_STATE.ESP_SEND){
			packets+=1;

			latencies.push(lat)
			// console.log(diff)s
			bytes+=JSON.stringify(arg).length;
			if(JSON.stringify(arg).length < 20){
				console.log(arg)
			}
		}else{
			console.log("Reset evenviado!")
			socket.emit("reset","");
		}		
	});

	

	// socket.disconnect();
});

app.use(express.json());

app.use(routes);

export { http, io };
