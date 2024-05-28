(define (domain Kbox-4)
(:requirements :strips :typing)
;;BestCase
(:types pos agent box push time)
(:constants
 p1-1 - pos
 p1-2 - pos
 p2-1 - pos
 p2-2 - pos
 p3-1 - pos
 p3-2 - pos
 p4-1 - pos
 p4-2 - pos
 p5-1 - pos
 p5-2 - pos
 b0 - box
 b1 - box
 b2 - box
 a1 - agent
 a2 - agent
 a3 - agent
 t1 - time
 t2 - time
 t3 - time
 t4 - time
 t5 - time
 t6 - time
 t7 - time
 t8 - time
 t9 - time
 t10 - time
 t11 - time
 t12 - time
 t13 - time
 t14 - time
 t15 - time
 t16 - time
 t17 - time
 t18 - time
 t19 - time
 t20 - time
 t21 - time
 t22 - time
 t23 - time
 t24 - time
 t25 - time
 t26 - time
 t27 - time
 t28 - time
 t29 - time
 t30 - time
 t31 - time
 t32 - time
 t33 - time
 t34 - time
 t35 - time
 t36 - time
 t37 - time
 t38 - time
 t39 - time
 t40 - time
 t41 - time
 t42 - time
 t43 - time
 t44 - time
 t45 - time
 t46 - time
 t47 - time
 t48 - time
 t49 - time
 t50 - time
 t51 - time
 t52 - time
 t53 - time
 t54 - time
 t55 - time
 t56 - time
 t57 - time
 t58 - time
 t59 - time
 t60 - time
 t61 - time
 t62 - time
 t63 - time
 t64 - time
 t65 - time
 t66 - time
 t67 - time
 t68 - time
 t69 - time
 t70 - time
 t71 - time
 t72 - time
 t73 - time
 t74 - time
 t75 - time
 t76 - time
 t77 - time
 t78 - time
 t79 - time
 t80 - time
 t81 - time
 t82 - time
 t83 - time
 t84 - time
 t85 - time
 t86 - time
 t87 - time
 t88 - time
 t89 - time
 t90 - time
 t91 - time
 t92 - time
 t93 - time
 t94 - time
 t95 - time
 t96 - time
 t97 - time
 t98 - time
 t99 - time
 t100 - time
 t101 - time
 t102 - time
 t103 - time
 t104 - time
 t105 - time
 t106 - time
 t107 - time
 t108 - time
 t109 - time
 t110 - time
 t111 - time
 t112 - time
 t113 - time
 t114 - time
 t115 - time
 t116 - time
 t117 - time
 t118 - time
 t119 - time
 t120 - time
 t121 - time
 t122 - time
 t123 - time
 t124 - time
 t125 - time
 t126 - time
 t127 - time
 t128 - time
 t129 - time
 t130 - time
 t131 - time
 t132 - time
 t133 - time
 t134 - time
 t135 - time
 t136 - time
 t137 - time
 t138 - time
 t139 - time
 t140 - time
 t141 - time
 t142 - time
 t143 - time
 t144 - time
 t145 - time
 t146 - time
 t147 - time
 t148 - time
 t149 - time
 t150 - time
 t151 - time
 t152 - time
 t153 - time
 t154 - time
 t155 - time
 t156 - time
 t157 - time
 t158 - time
 t159 - time
 t160 - time
 t161 - time
 t162 - time
 t163 - time
 t164 - time
 t165 - time
 t166 - time
 t167 - time
 t168 - time
 t169 - time
 t170 - time
 t171 - time
 t172 - time
 t173 - time
 t174 - time
 t175 - time
 t176 - time
 t177 - time
 t178 - time
 t179 - time
 t180 - time
 t181 - time
 t182 - time
 t183 - time
 t184 - time
 t185 - time
 t186 - time
 t187 - time
 t188 - time
 t189 - time
 t190 - time
 t191 - time
 t192 - time
 t193 - time
 t194 - time
 t195 - time
 t196 - time
 t197 - time
 t198 - time
 t199 - time
 t200 - time
)


(:predicates
(adj ?i - pos ?j - pos)
(agent-at ?a - agent ?i - pos)
(Kbox-at ?b - box ?i - pos)
(KNbox-at ?b - box ?i - pos)
(heavy ?b - box)
(same-agent ?a1 - agent ?a2 - agent)
(current-time ?t - time)
(next-time ?t1 - time ?t2 - time)
(active-agent ?a - agent)
)

(:action no-op
 :parameters (?t1 - time ?t2 - time )
 :precondition 
(and (next-time ?t1 ?t2) (current-time ?t1))
 :effect 
(and (not (current-time ?t1)) (current-time ?t2) 
	(when (current-time t2) 
		(and (not (KNbox-at b0 p1-2)) (Kbox-at b0 p1-2)))
	(when (current-time t1) 
		(and (not (KNbox-at b1 p3-2)) (Kbox-at b1 p3-2)))
	(when (current-time t1) 
		(and (not (KNbox-at b2 p5-2)) (Kbox-at b2 p5-2))))
)
(:action move
 :parameters (?start - pos ?end - pos ?a - agent ?t1 - time ?t2 - time )
 :precondition 
(and (adj ?start ?end) (agent-at ?a ?start) (next-time ?t1 ?t2) (current-time ?t1) (active-agent ?a))
 :effect 
(and (not (agent-at ?a ?start)) (agent-at ?a ?end) (not (current-time ?t1)) (current-time ?t2) 
	(when (current-time t2) 
		(and (not (KNbox-at b0 p1-2)) (Kbox-at b0 p1-2)))
	(when (current-time t1) 
		(and (not (KNbox-at b1 p3-2)) (Kbox-at b1 p3-2)))
	(when (current-time t1) 
		(and (not (KNbox-at b2 p5-2)) (Kbox-at b2 p5-2))))
)
(:action push
 :parameters (?start - pos ?end - pos ?b - box ?a - agent ?t1 - time ?t2 - time )
 :precondition 
(and (adj ?start ?end) (agent-at ?a ?start) (not (KNbox-at ?b ?start)) (Kbox-at ?b ?start) (not (heavy ?b)) (next-time ?t1 ?t2) (current-time ?t1) (active-agent ?a))
 :effect 
(and (not (Kbox-at ?b ?start)) (KNbox-at ?b ?start) (not (KNbox-at ?b ?end)) (Kbox-at ?b ?end) (not (current-time ?t1)) (current-time ?t2) 
	(when (current-time t2) 
		(and (not (KNbox-at b0 p1-2)) (Kbox-at b0 p1-2)))
	(when (current-time t1) 
		(and (not (KNbox-at b1 p3-2)) (Kbox-at b1 p3-2)))
	(when (current-time t1) 
		(and (not (KNbox-at b2 p5-2)) (Kbox-at b2 p5-2))))
)
(:action joint-push
 :parameters (?start - pos ?end - pos ?b - box ?a1 - agent ?a2 - agent ?t1 - time ?t2 - time )
 :precondition 
(and (adj ?start ?end) (agent-at ?a1 ?start) (not (KNbox-at ?b ?start)) (Kbox-at ?b ?start) (heavy ?b) (not (same-agent ?a1 ?a2)) (next-time ?t1 ?t2) (current-time ?t1) (active-agent ?a1))
 :effect 
(and (not (Kbox-at ?b ?start)) (KNbox-at ?b ?start) (not (KNbox-at ?b ?end)) (Kbox-at ?b ?end) (not (current-time ?t1)) (current-time ?t2) 
	(when (current-time t2) 
		(and (not (KNbox-at b0 p1-2)) (Kbox-at b0 p1-2)))
	(when (current-time t1) 
		(and (not (KNbox-at b1 p3-2)) (Kbox-at b1 p3-2)))
	(when (current-time t1) 
		(and (not (KNbox-at b2 p5-2)) (Kbox-at b2 p5-2))))
)
(:action observe-boxT
 :parameters (?i - pos ?a - agent ?b - box ?t1 - time ?t2 - time )
 :precondition 
(and (agent-at ?a ?i) (next-time ?t1 ?t2) (current-time ?t1) (active-agent ?a) (not (Kbox-at ?b ?i)) (not (KNbox-at ?b ?i)))
 :effect 
(and (not (current-time ?t1)) (current-time ?t2) 
	(when (current-time t2) 
		(and (Kbox-at b0 p1-2) (Kbox-at b0 p1-2)))
	(when (current-time t1) 
		(and (Kbox-at b1 p3-2) (Kbox-at b1 p3-2)))
	(when (current-time t1) 
		(and (Kbox-at b2 p5-2) (Kbox-at b2 p5-2)))(Kbox-at ?b ?i))
)
(:action observe-boxF
 :parameters (?i - pos ?a - agent ?b - box ?t1 - time ?t2 - time )
 :precondition 
(and (agent-at ?a ?i) (next-time ?t1 ?t2) (current-time ?t1) (active-agent ?a))
 :effect 
(and (not (current-time ?t1)) (current-time ?t2) 
	(when (current-time t2) 
		(and (Kbox-at b0 p1-2) (Kbox-at b0 p1-2)))
	(when (current-time t1) 
		(and (Kbox-at b1 p3-2) (Kbox-at b1 p3-2)))
	(when (current-time t1) 
		(and (Kbox-at b2 p5-2) (Kbox-at b2 p5-2)))(KNbox-at ?b ?i))
)
(:action R0
 :parameters (?t1 - time )
 :precondition 
(and (not (KNbox-at b0 p1-1)) (Kbox-at b0 p1-1) (current-time ?t1))
 :effect 
(and (not (Kbox-at b0 p1-2)) (KNbox-at b0 p1-2))
)
(:action R1
 :parameters (?t1 - time )
 :precondition 
(and (not (KNbox-at b0 p1-2)) (Kbox-at b0 p1-2) (current-time ?t1))
 :effect 
(and (not (Kbox-at b0 p1-1)) (KNbox-at b0 p1-1))
)
(:action R2
 :parameters (?t1 - time )
 :precondition 
(and (not (KNbox-at b1 p3-1)) (Kbox-at b1 p3-1) (current-time ?t1))
 :effect 
(and (not (Kbox-at b1 p3-2)) (KNbox-at b1 p3-2))
)
(:action R3
 :parameters (?t1 - time )
 :precondition 
(and (not (KNbox-at b1 p3-2)) (Kbox-at b1 p3-2) (current-time ?t1))
 :effect 
(and (not (Kbox-at b1 p3-1)) (KNbox-at b1 p3-1))
)
(:action R4
 :parameters (?t1 - time )
 :precondition 
(and (not (KNbox-at b2 p5-1)) (Kbox-at b2 p5-1) (current-time ?t1))
 :effect 
(and (not (Kbox-at b2 p5-2)) (KNbox-at b2 p5-2))
)
(:action R5
 :parameters (?t1 - time )
 :precondition 
(and (not (KNbox-at b2 p5-2)) (Kbox-at b2 p5-2) (current-time ?t1))
 :effect 
(and (not (Kbox-at b2 p5-1)) (KNbox-at b2 p5-1))
)
)
