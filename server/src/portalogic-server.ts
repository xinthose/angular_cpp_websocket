import { createServer, Server } from "http";

import * as express from "express";
import * as socketIo from "socket.io";
import * as bodyParser from "body-parser";
import * as cors from "cors";

export class PortalogicServer {
    public static readonly PORT: number = 8090;
    private app: express.Application;
    private server: Server;
    private io: SocketIO.Server;
    private port: string | number;

    constructor() {
        // create app
        this.app = express();
        this.app.use(cors());
        this.app.use(bodyParser.json());
        this.app.use(bodyParser.urlencoded({
            extended: true
        }));

        // create server
        this.port = process.env.PORT || PortalogicServer.PORT;
        this.server = createServer(this.app);

        // setup websockets 
        this.io = socketIo(this.server);
        this.setup_routes();
        this.listen();
    }

    private setup_routes(): void {
        this.app.post("/send_email", function (req, res) {
            let debug = false;
            console.log("send_email");

        });
    }

    private listen(): void {
        this.server.listen(this.port, () => {
            console.log("Running server on port %s", this.port);
        });

        this.io.on("connect", (socket: any) => {
            console.log("Connected client on port %s.", this.port);

            // setup event handles
            socket.on("disconnect", (req: any) => {
                console.log("Client disconnected");
            });

            socket.on("station_closed", (req: any) => {
                console.log("station_closed");
                this.io.emit("station_closed");
            });

            socket.on("station_open", (req: any) => {
                console.log("station_open");
                this.io.emit("station_open");
            });
        });
    }

    public getApp(): express.Application {
        return this.app;
    }
}
