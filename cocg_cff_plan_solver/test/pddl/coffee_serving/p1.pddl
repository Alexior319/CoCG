(define (problem CM_R1_C1)
    (:domain coffee_serving)

    (:objects
        r1 - robot
        ps p1 p2 pg - location
        c1 - coffee
    )

    (:init
        (robot_at r1 ps)
        (hand_empty r1)
        (unknown
            (coffee_at c1 p1))
        (unknown
            (coffee_at c1 p2))

        (oneof
            (coffee_at c1 p1)
            (coffee_at c1 p2)
        )
    )
    (:goal
        (and
            (coffee_at c1 pg)
        )
    )
)