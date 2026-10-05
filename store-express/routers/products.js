import express from "express";

const router = express.Router();


router.get("/", (req, res) => {
    res.json({
        ok: true,
        message: "Welcome"
    });
});

router.get("/items", async (req, res) => {
    const response = await fetch("https://fakestoreapi.com/products?limit=5");
    const items = await response.json();

    res.json({ ok: true, items });
})
router.get("/categories", async (req, res) => {
    const response = await fetch("https://fakestoreapi.com/products/categories");
    const categories = await response.json();

   res.json({ ok:true, categories})
});
export default router;