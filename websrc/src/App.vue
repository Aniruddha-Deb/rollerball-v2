<script setup>
import ChessBoard from 'chessboardjs-vue3';
import Board from './components/Board.vue'
import {onMounted, reactive, watch, computed} from 'vue';

const state = reactive({

    sockets : {
        white : {
            socket : null,
            address : 'localhost:8181',
            state : 'disconnected'
        },
        black : {
            socket : null,
            address : 'localhost:8182',
            state : 'disconnected'
        }
    },

    game : {
        type  : 'board-7-3',
        state : 'idle',
        curr_player : 'white',
        move_list : [],
        position_list : []
    },

    players : {
        white : 'waiting',
        black : 'waiting'
    },

    timer : {
        time_limit: 60,
        white : {
            time_ms : 0
        },
        black : {
            time_ms : 0
        }
    },

    info : {
        left  : 'Rollerball v.2.0',
        right : 'Rev. Oct 24, 2023'
    }

});

var timer_interval = null;

const INTERVAL = 10;

function send_message(side, message) {

    if (state.sockets[side].state === 'connected') {
        state.sockets[side].socket.send(message);
    }
    else {
        stop_game(`The ${side} side disconnected arbitrarily`);
    }

}

function process_message(side, message) {

    var tokens = message.trim().split(' ');
    var command = tokens[0];

    if      (command === 'uciok')     on_uciok(side, tokens);
    else if (command === 'newgameok') on_newgameok(side, tokens);
    else if (command === 'bestmove')  on_bestmove(side, tokens);
    else {
        stop_game(`Unknown command ${command} received from ${side}`);
    }
}

function connect_socket(side) {

    state.sockets[side].state  = 'connecting';
    state.sockets[side].socket = new WebSocket(`ws://${state.sockets[side].address}`);

    state.sockets[side].socket.onopen = (event) => {
        state.sockets[side].socket.send("uci");
    }

    state.sockets[side].socket.onerror = (err) => {
        state.right_info = `Could not connect to ${side} bot`;
        state.sockets[side].state = 'disconnected';
    }
    
    state.sockets[side].socket.onmessage = (msg) => {
        process_message(side, msg.data);
    }
}

function disconnect_socket(side) {
    state.sockets[side].socket.close();
    state.sockets[side].state = 'disconnected';
}

function tick() {

    if (state.game.state === 'white_thinking') {
        state.timer.white.time_ms -= INTERVAL;
    }
    else if (state.game.state === 'black_thinking') {
        state.timer.black.time_ms -= INTERVAL;
    }

    if (state.timer.white.time_ms <= 0) {
        stop_game('White lost by timeout');
    }
    else if (state.timer.black.time_ms <= 0) {
        stop_game('Black lost by timeout');
    }

}

function start_game() {

    state.timer.white.time_ms = state.timer.time_limit * 1000;
    state.timer.black.time_ms = state.timer.time_limit * 1000;
    timer_interval = setInterval(tick, INTERVAL);
    state.game.state = 'starting';
    send_message('black', `ucinewgame ${state.game.type} ${state.timer.time_limit}`);
    send_message('white', `ucinewgame ${state.game.type} ${state.timer.time_limit}`);

}

function stop_game(message = 'Something bad happened') {

    send_message('black', 'quit');
    send_message('white', 'quit');
    clearInterval(timer_interval);
    state.game.state = 'final';
    state.info.right = message;
    state.players.white = state.players.black = 'waiting';
}

function reset_game() {

    state.game.state = 'ready'
    state.game.curr_player = 'white'
    state.game.move_list.splice(0);
    state.game.position_list.splice(0);

    state.info.left = 'Rollerball v.2.0';
    state.info.right = 'Rev. Oct 24, 2023';
}

function on_uciok(side, tokens) {
    state.sockets[side].state = 'connected';
    if (state.sockets.white.state === state.sockets.black.state && 
        state.sockets.white.state === 'connected') {
        state.game.state = 'ready';
    }
}

function on_newgameok(side, tokens) {

    state.players[side] = 'ready';

    if (state.players.white === 'ready' && state.players.black === 'ready') {
        state.game.state = 'white_thinking';
        state.players.white = 'thinking';
        ask_for_move('white');
    }
}

function ask_for_move(side) {
    var move_message = `position startpos moves ${state.game.move_list.join(' ')}`;
    send_message(side, move_message);
    send_message(side, `go ${state.timer[side].time_ms}`);
}

function on_bestmove(side, tokens) {

    if (state.game.state !== 'white_thinking' && 
        state.game.state !== 'black_thinking') {
        // silently discard
        return;
    }

    var move = tokens[1];

    // check if it's a null move. If yes, declare checkmate/stalemate
    if (move === '0000') {
        stop_game(`${side} gave a null move, indicating either Checkmate or Stalemante`);
        return;
    }

    state.game.move_list.push(move);

    // check draw conditions - 3fold repetition (can skip for now?)

    if (side === 'white') {
        state.players.white = 'ready';
        state.players.black = 'thinking';
        state.game.state = 'black_thinking';
        ask_for_move('black');
    }
    else if (side === 'black') {
        state.players.white = 'thinking';
        state.players.black = 'ready';
        state.game.state = 'white_thinking';
        ask_for_move('white');
    }
    else {
        stop_game(`Bad side ${side} passed to on_bestmove`);
    }
}

//--- Dependencies ------------------------------------------------------------

function disable_on_disconnect(s) {
    if (s === 'disconnected' || s === 'connecting') {
        state.game.state = 'idle';
    }
}

watch(() => state.sockets.white.state, disable_on_disconnect);
watch(() => state.sockets.black.state, disable_on_disconnect);
// watch(() => state.game.state, (state) => {console.log(state)});

//-- UI Stuff ----------------------------------------------------------------- 


function socket_btn_disabled_closure(side) {
    return () => {
        if ((state.game.state === 'idle'  || 
             state.game.state === 'ready') && 
             state.sockets[side].state !== 'connecting') {
            return false;
        }
        return true;
    }
}

function socket_btn_class_closure(side) {

    return () => {
        if (state.sockets[side].state === 'connected') return 'button-green';
        else if (state.sockets[side].state === 'connecting') return 'button-red';
        else if (state.sockets[side].state === 'disconnected') return 'button-yellow';
        else {
            console.log('ERROR: Invalid socket state');
            return 'button-to-connect';
        }
    }
}

function socket_btn_text_closure(side) {

    return () => {
        if (state.sockets[side].state === 'connected') return `Disconnect ${side} bot`;
        else if (state.sockets[side].state === 'connecting') return `Connecting ${side} bot`;
        else if (state.sockets[side].state === 'disconnected') return `Connect ${side} bot`;
        else {
            return 'ERROR: Invalid socket state';
        }
    }
}

const white_socket_btn_disabled = computed(socket_btn_disabled_closure('white'));
const black_socket_btn_disabled = computed(socket_btn_disabled_closure('black'));

const white_socket_btn_class = computed(socket_btn_class_closure('white'));
const black_socket_btn_class = computed(socket_btn_class_closure('black'));

const white_socket_btn_text = computed(socket_btn_text_closure('white'));
const black_socket_btn_text = computed(socket_btn_text_closure('black'));

function socket_btn_click(side) {
    if (state.sockets[side].state === 'connected') disconnect_socket(side);
    else connect_socket(side);
}

const game_btn_class = computed(() => {
    
    if (state.game.state === 'starting') {
        return 'button-red';
    }
    else if (state.game.state === 'white_thinking' || 
             state.game.state === 'black_thinking' ) {
        return 'button-green';
    }
    else return 'button-yellow';

});

const game_btn_text = computed(() => {

    if (state.game.state === 'white_thinking' || 
        state.game.state === 'black_thinking') {
        return 'Stop Game';
    }
    else if (state.game.state === 'final') {
        return 'Reset Game';
    }
    else if (state.game.state === 'starting') {
        return 'Starting Game';
    }
    else return 'Start Game';

});

const game_btn_disabled = computed(() => {
    if (state.game.state === 'idle' || state.game.state === 'starting') return true;
    return false;
});

function game_btn_click() {

    if (state.game.state === 'ready') {
        start_game();
    }
    else if (state.game.state === 'white_thinking' || 
             state.game.state === 'black_thinking') {
        stop_game('Game stopped by user');
    }
    else if (state.game.state === 'final') {
        reset_game();
    }
}

const board_btns_disabled = computed(() => {
    if (state.game.state === 'ready' || state.game.state === 'idle') return false;
    return true;
});

function set_board_type(value) {
    state.game.type = value;
}

function time_closure(side) {

    return () => {
        var classes = ['time-left'];
        if ((state.game.state === 'white_thinking' && side === 'white') ||
            (state.game.state === 'black_thinking' && side === 'black')) {
            classes.push('active');
        }
        return classes;
    }
}

const timer_classes = computed(() => {
    var classes = ['timer'];
    if (!(state.game.state === 'white_thinking' ||
          state.game.state === 'black_thinking' || 
          state.game.state === 'final')) {
        classes.push('hidden');
    }
    return classes;
});

const white_timer_classes = computed(time_closure('white'));
const black_timer_classes = computed(time_closure('black'));

</script>

<template>

<form>
    <div class='field-pad'>
        <label for="white_address">White Address:</label>
        <input id="white_address" v-model="state.sockets.white.address" placeholder="addr:port">
    </div>
    <div class='field-pad'>
        <label for="black_address">Black Address:</label>
        <input id="black_address" v-model="state.sockets.black.address" placeholder="addr:port">
    </div>
    <div class='field-pad'>
        <label for="time_limit">Time Limit:</label>
        <input type="number" id="time_limit" v-model.number="state.timer.time_limit" placeholder="sec">
    </div>
</form>
<div class='button-bar'>
    <div class='button-enclose'>
        <button :disabled='white_socket_btn_disabled' 
                :class='white_socket_btn_class' 
                @click="socket_btn_click('white')" 
                v-html="white_socket_btn_text"></button>
    </div>
    <div class='button-enclose'>
        <button :disabled='black_socket_btn_disabled' 
                :class='black_socket_btn_class' 
                @click="socket_btn_click('black')" 
                v-html="black_socket_btn_text"></button>
    </div>
    <div class='button-enclose'>
        <button :disabled="game_btn_disabled" 
                :class='game_btn_class' 
                @click="game_btn_click" 
                v-html="game_btn_text"></button>
    </div>
</div>
<div :class='timer_classes'>
    <div :class='white_timer_classes'>White: 
        <span class='time'>{{(state.timer.white.time_ms / 1000).toFixed(1)}}</span>
    </div>
    <div :class='black_timer_classes'>Black: 
        <span class='time'>{{(state.timer.black.time_ms / 1000).toFixed(1)}}</span>
    </div>
</div>

<Board ref='board' :type='state.game.type' :moves="state.game.move_list" id='myBoard'/>

<div class='button-bar'>
    <div class='button-enclose'>
        <button :disabled='board_btns_disabled' 
                 class='button-yellow' 
                @click="set_board_type('board-7-3')">7_3 board</button>
    </div>
    <div class='button-enclose'>
        <button :disabled='board_btns_disabled' 
                 class='button-yellow' 
                @click="set_board_type('board-8-4')">8_4 board</button>
    </div>
    <div class='button-enclose'>
        <button :disabled='board_btns_disabled' 
                 class='button-yellow' 
                @click="set_board_type('board-8-2')">8_2 board</button>
    </div>
</div>

<div class='info-bar'>
    <label class='left-info' v-html="state.info.left"></label>
    <label class='right-info' v-html="state.info.right"></label>
</div>
</template>

<style scoped>
form {
  display: flex;
  width: 60%;
}

div.button-bar {
  padding-top: 20px;
  display: flex;
  flex-direction: row;
  width: 60%;
  padding-bottom: 20px;
}

div.timer {
  display: flex;
  flex-direction: row;
  width: 60%;
}

div.time-left {
    flex-grow: 1;
    text-align: center;
    font-weight: bold;
    font-size: 14pt;
}

.hidden {
    visibility: hidden;
}

.time {
    font-family: monospace;
    font-weight: bold;
    font-size: 14pt;
}

.active {
    color: var(--vt-c-white);
}

div.book-container {
  display: flex;
  flex-direction: column;
  flex-grow: 1;
}

div.button-enclose {
  flex-grow: 1;
  padding-right: 10px;
  padding-left: 10px;
  text-align: center;
}

button.button-green {
  background-color: ForestGreen;
}

button.button-yellow {
  background-color: GoldenRod;
}

button.button-red {
  background-color: Crimson;
}

button:hover {
  filter: brightness(90%);
}

button:active {
  filter: brightness(80%);
}

button:disabled {
  filter: brightness(40%);
}

button {
  width: 200px;
  min-width: 150px;
  height: 30px;
  border: none;
  font-weight: bold;
  background-color: green;
  color: var(--vt-c-white);
  border-radius: 4px;
}

div.field-pad {
  width: 100%;
  padding-right: 8px;
  padding-left: 8px;
  align: center;
  text-align: center;
}

div.info-bar {
  width: 60%;
  display: flex;
  flex-direction: row;
}

label.left-info {
  text-align: left;
  flex-grow: 1;
}

label.right-info {
  text-align: right;
  flex-grow: 1;
}

label {
  padding-right: 4px;
}

input[type=number] {
  width: 60px;
}

input {
  width: 150px;
  padding: 4px;
  font-family: monospace;
  background-color: var(--vt-c-black);
  border: none;
  text-align: center;
  border-bottom: solid 2px var(--vt-c-divider-dark-1);
  color: var(--vt-c-white);
  -moz-appearance: textfield;
}
</style>
