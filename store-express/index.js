import express from 'express'
import dotenv from 'dotenv'
import appRouter from  "./routers/app.js";
import productsRouter from  "./routers/products.js";
dotenv.config()

const app = express();
const PORT = process.env.PORT 

app.set("view engine", "ejs")
app.set("views", "./pages")

app.use("/", appRouter);
app.use("/products", productsRouter);

app.listen( PORT ,() => {
    console.log("server started ...")
})