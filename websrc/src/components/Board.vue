<script setup>
import ChessBoard from 'chessboardjs-vue3';
import {onMounted, reactive, computed, defineExpose, defineProps, watch} from 'vue';

const state = reactive({
    board : null,
    prev_move_list_str : ""
})

onMounted(() => {
    state.board = new ChessBoard(props.id);
    reset(props.type);
})

const props = defineProps({
    type: String,
    id: String,
    moves: Object
});

const enclosing_class = computed(() => {
    return 'enclose-' + props.type; 
})

const pos_7_3 = {
    c1: 'wP',
    c2: 'wP',
    d1: 'wB',
    d2: 'wK',
    e1: 'wR',
    e2: 'wR',

    c6: 'bR',
    c7: 'bR',
    d6: 'bK',
    d7: 'bB',
    e6: 'bP',
    e7: 'bP'
}

const pos_8_4 = {
    c1: 'wP',
    c2: 'wP',
    d1: 'wB',
    d2: 'wK',
    e1: 'wR',
    e2: 'wR',
    f1: 'wP',
    f2: 'wP',

    c7: 'bP',
    c8: 'bP',
    d7: 'bR',
    d8: 'bR',
    e7: 'bK',
    e8: 'bB',
    f7: 'bP',
    f8: 'bP'
}

const pos_8_2 = {
    c1: 'wP',
    c2: 'wP',
    c3: 'wP',
    d2: 'wN',
    d3: 'wN',
    e2: 'wK',
    e3: 'wB',
    f1: 'wR',
    f2: 'wR',
    f3: 'wP',

    c8: 'bR',
    c7: 'bR',
    c6: 'bP',
    d7: 'bK',
    d6: 'bB',
    e7: 'bN',
    e6: 'bN',
    f8: 'bP',
    f7: 'bP',
    f6: 'bP',
}

watch(() => props.type, reset);

watch(props.moves, update_moves);

function do_move(move) {
    if (move.length > 4) {
        // promote - this is harder than it needs to be
        var curr_pos = state.board.position();
        var piece_to_promote = curr_pos[move.slice(0,2)];
        delete curr_pos[move.slice(0,2)];
        curr_pos[move.slice(2,4)] = piece_to_promote[0] + move[4].toUpperCase();
        state.board.position(curr_pos, true);
    }
    else {
        var textmove = move.slice(0,2)+"-"+move.slice(2,4);
        state.board.move(textmove);
    }
}

function reset(type) {
    var sp = pos_7_3;
    if (type === 'board-8-4') sp = pos_8_4;
    else if (type === 'board-8-2') sp = pos_8_2;
    state.prev_move_list_str = '';

    if (state.board !== null) {
        state.board.position(sp, false);
    }
}

function update_moves(moves) {

    if (moves.length === 0) {
        reset(props.type);
        return;
    }

    // do a diff on the previous move list: 
    var move_list_str = moves.join(' ');
    var extra_moves = [];
    if (state.prev_move_list_str === 
                move_list_str.substring(0, state.prev_move_list_str.length)) {
        // prefixes match. Only need to move extraneous stuff
        extra_moves = move_list_str.substring(state.prev_move_list_str.length)
                                   .trim()
                                   .split(' ');
    }

    extra_moves.forEach(do_move);
    state.prev_move_list_str = move_list_str;
}
</script>

<template>
<div :class='enclosing_class'>
    <div :id="props.id" :class="props.type" style="width: 400px"></div>
</div>
</template>

<style scoped>

div.enclose-board-7-3 {
  padding-left: 50px;
  padding-bottom: 20px;
}

div.enclose-board-8-4 {
  padding-left: 0px;
  padding-bottom: 20px;
  padding-top: 20px;
}

div.enclose-board-8-2 {
  padding-left: 0px;
  padding-bottom: 20px;
  padding-top: 20px;
}

.board-7-3 {
  clip-path: polygon(
      0px 65px, 
      15px 50px, 
      100px 50px, /* */
      100px 300px,
      250px 300px,
      250px 150px,
      100px 150px,
      100px 50px, /* */
      335px 50px, 
      350px 65px, 
      350px 385px, 
      335px 400px, 
      15px 400px, 
      0px 385px
  );
}

.board-8-4 {
  clip-path: polygon(
      0px 15px, 
      15px 0px, 
      100px 0px, /* */
      100px 300px,
      300px 300px,
      300px 100px,
      100px 100px,
      100px 0px, /* */
      385px 0px, 
      400px 15px, 
      400px 385px, 
      385px 400px, 
      15px 400px, 
      0px 385px
  );
}

.board-8-2 {
  clip-path: polygon(
      0px 15px, 
      15px 0px, 
      150px 0px, /* */
      150px 250px,
      250px 250px,
      250px 150px,
      150px 150px,
      100px 0px, /* */
      385px 0px, 
      400px 15px, 
      400px 385px, 
      385px 400px, 
      15px 400px, 
      0px 385px
  );
}
</style>
