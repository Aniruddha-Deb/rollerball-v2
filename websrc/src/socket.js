// TODO make a UCI-esque protocol here to communicate with engines

import { reactive } from "vue";
import { io } from "socket.io-client";

export const state = reactive({
  connected: false,
  white_moves: [],
  black_moves: []
});

// "undefined" means the URL will be computed from the `window.location` object
const URL = process.env.NODE_ENV === "production" ? undefined : "http://localhost:3000";

export const socket_white = io("http://localhost:8181");
export const socket_black = io("http://localhost:8182");

socket_white.on("connect", () => {
  state.connected = true;
});

socket_white.on("disconnect", () => {
  state.connected = false;
});

socket_black.on("uciok", (...args) => {});
socket_white.on("uciok", (...args) => {});
