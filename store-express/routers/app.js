
import express from "express";

const router = express.Router();

router.get("/", async (req, res) => {
    const response = await fetch("https://fakestoreapi.com/products");
    const products = await response.json();

    res.json({ ok: true, products });
});

router.get("/about", (req, res) => {
    res.render("about");
});

router.get("/contacts", (req, res) => {
    res.render("contacts");
});

export default router;

