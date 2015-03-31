 {
    "targets": [
        {
            "target_name": "window-info",
            "sources": [ "window-info.cc" ],
            "conditions": [
                [
                    "OS=='win'",
                    {
                        "link_settings": {
                            "libraries": [ 
                                "psapi.lib"
                            ]
                        }
                    }

                ]
            ]

        }
    ]
}