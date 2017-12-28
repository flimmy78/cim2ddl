package com.znd.ei.ads.web.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;

@Controller
public class DefaultController {

    @GetMapping("/")
    public String home1() {
        return "/home";
    }

    @GetMapping("/home")
    public String home() {
        return "/home";
    }


    @GetMapping("/pradequacy")
    public String about() {
        return "/pradequacy";
    }

    @GetMapping("/model")
    public String login() {
        return "/model";
    }

    @GetMapping("/403")
    public String error403() {
        return "/error/403";
    }

}
